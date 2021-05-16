
#include <stack>
#include "../library/class_beam_stack_item.cxx"
stack<beam_stack_item*> beam_stack;

void PruneLayer(myPriorityQueue* q, int w, HHashtable* qh) {
  if (q->size() <= w) {
    return;
  }
  PuzState* s;
  while (q->size() > w) {
	s = q->popLast();
	qh->pop(s);
	if (q->size() > w) {
	  delete s;
	  CUR_MEM--;
    }
  }
  beam_stack_item* t = beam_stack.top();
  t->f_max = s->GetFn();
  t->SetMaxPath(s->path);
  delete s;
  CUR_MEM--;
}

void GenerateChildrenBS(PuzState* state, myPriorityQueue* q, myPriorityQueue* open[], short* minFound, int U, HHashtable* qh, int w) {
	NUM_NODES_EXPANDED++;
  //assert(beam_stack.size() == state->GetDepth() + 2);
  beam_stack_item* t = beam_stack.top();
  //assert(t->f_min <= t->f_max);
  
  for (int i = 0; i < 4; i++) {
    if (!state->IsChildPossible(i)) {
	  continue;
	}
   	PuzState *childstate = state->getChild(i, state->GetHn());
    if (childstate->GetFn() < t->f_min || childstate->GetFn() > t->f_max || childstate->GetFn() >= U) {
	  // never do the below line.. it will disrupt starting from the min_path below.. so the optimization is not applicable for beam stack
	  // || (q->size() > w && q->bottom()->GetFn() <= childstate->GetFn())) {
	  delete childstate;
	  continue;
	}
	int index;
    List* qht = qh->getList(childstate);
    if (qht && (index = qht->getIndex(childstate)) != -1) {
      PuzState* oldState = qht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		int d = oldState->GetDepth();
	    myPriorityQueue* p = open[d]->getLink(oldState);
        open[d]->deleteLink(p);
		delete oldState;
		q->push(childstate);
		qht->updateIndex(index, childstate);
	  } else {
		delete childstate;
	  }
	} else {
      q->push (childstate);
      if (qht) {
		qht->push(childstate);
	  } else {
        qh->push (childstate);
      }
      CUR_MEM++;
      if (CUR_MAX_MEM < CUR_MEM) {
		CUR_MAX_MEM = CUR_MEM;
	  }
    }
  }
  
  if (!(*minFound) && t->min_path[1] != -1) {
    myPriorityQueue* temp = q;
    temp = temp->getNext();
    PuzState* kt = temp->getState();
    while (kt != NULL && !t->EqualsMinPath(kt->path)) {
	  temp = temp->getNext();
	  if (kt->GetFn() == t->f_min) {
	    q->deleteLink(temp->getPrev());
	    qh->pop(kt);
	    delete kt;
	    CUR_MEM--;
      }
      kt = temp->getState();
    }
    if (kt != NULL && t->EqualsMinPath(kt->path)) {
	  *minFound = 1;
	}
  }
}

int search(myPriorityQueue* open[], int l, int U, int w, time_t start_time, FILE* fp, HHashtable* qh) {
  //assert(beam_stack.size() == l + 1);
  //time_t end_time;
  for (int i = l; i < U; i++) {
    while (!open[i]->empty()) {
      PuzState* s = open[i]->popLast();
      qh->pop(s);
      delete s;
      CUR_MEM--;
    }
  }
  l = l - 1;
  
  while (l < U && !open[l]->empty()) {
	myPriorityQueue* temp = open[l];
    temp = temp->getNext();
    short minFound = 0;
	while (temp->getState() != NULL) {
	  PuzState* node = temp->getState();
      time_t end_time = time(NULL);
	  if (node->IsGoalState()) {
        U = node->GetFn();
	    fprintf(fp, "%d %lf %ld %ld %d\n", U, (OPT * 100.0) / U, CUR_MAX_MEM, NUM_NODES_EXPANDED, (int)(end_time - start_time));
        fflush(fp);
		break;
	  }
	  if (int(end_time - start_time) > MAX_T) {
		return U;  
	  }
	  GenerateChildrenBS(node, open[l + 1], open, &minFound, U, qh, w);
	  
	  temp = temp->getNext();
	}
	if (l + 1 < MAX_D) {
	  PruneLayer(open[l + 1], w, qh);
	}
	l = l + 1;
    beam_stack.push(new beam_stack_item(0, U, l + 2));
  }
  return U;
}

int Beam_Stack(int beam, FILE* fp) {
  time_t start_time = time(NULL);
  
  myPriorityQueue* open[MAX_D];
  for (int i = 0; i < MAX_D; i++) {
	open[i] = new myPriorityQueue(1);
  }
  HHashtable* qh = new HHashtable();
  
  PuzState* s = CreateInitState();
  open[0]->push (s);
  qh->push(s);
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;

  int U = MAX_D;
  beam_stack.push(new beam_stack_item(0, U, 1));
  beam_stack.push(new beam_stack_item(0, U, 2));

  while (beam_stack.size() > 1) {
	U = search(open, beam_stack.size() - 1, U, beam, start_time, fp, qh);
	if (time(NULL) - start_time > MAX_T) {
	  while(beam_stack.size() > 1) {
	    beam_stack.pop();
	  }
	}
	while (beam_stack.size() > 1 && (beam_stack.top())->f_max >= U) {
	  beam_stack_item* t = beam_stack.top();
	  beam_stack.pop();
	  delete t;
	}
	if (beam_stack.size() <= 1) {
	  break;
	}
	beam_stack.top()->f_min = beam_stack.top()->f_max;
	beam_stack.top()->SetMinPath(beam_stack.top()->GetMaxPath());
	beam_stack.top()->f_max = U;
	beam_stack.top()->ResetMaxPath();
  }
  fprintf(fp, "%d %lf %ld %ld %d\n", U, (OPT * 100.0) / U, CUR_MAX_MEM, NUM_NODES_EXPANDED, (int)(time(NULL) - start_time));
  fflush(fp);
  
  beam_stack_item* t = beam_stack.top();
  beam_stack.pop();
  delete t;
  
  for (int i = 0; i < MAX_D; i++) {
    while (!open[i]->empty()) {
	  PuzState* s = open[i]->popLast();
      delete s;
    }
    delete open[i];  
  }
  qh->deleteAllEntries();
  delete qh;
  
  return U;
}
