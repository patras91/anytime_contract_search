
#include <stack>
#include "../library/class_beam_stack_item.cxx"
stack<beam_stack_item*> beam_stack;

void PruneLayer(myPriorityQueue* q, int w, HHashtable* qh) {
  if (q->size() <= w) {
    return;
  }
  TspState* s;
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

void GenerateChildrenBS(TspState* state, myPriorityQueue* q, myPriorityQueue* open[], short* minFound, int U, HHashtable* qh, int w) {
	NUM_NODES_EXPANDED++;
  //assert(beam_stack.size() == state->GetDepth() + 2);
  beam_stack_item* t = beam_stack.top();
  //assert(t->f_min <= t->f_max);
  
  for (int i = 1; i < nodes; i++) {
    if (state->IsChosen(i)) {
	  continue;
	}
   	TspState *childstate = state->GetNewChild(i);
	int index;
    List* qht = qh->getList(childstate);
    if (qht && (index = qht->getIndex(childstate)) != -1) {
      TspState* oldState = qht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		int d = oldState->GetDepth();
	    myPriorityQueue* p = open[d]->getLink(oldState);
        open[d]->deleteLink(p);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
    if (childstate->GetFn() < t->f_min || childstate->GetFn() > t->f_max || childstate->GetFn() >= U) {
	  // never do the below line.. it will disrupt starting from the min_path below.. so the optimization is not applicable for beam stack
	  // || (q->size() > w && q->bottom()->GetFn() <= childstate->GetFn())) {
	  delete childstate;
	  qht->popIndex(index);
	  continue;
	}
		q->push(childstate);
		qht->updateIndex(index, childstate);
	  } else {
		delete childstate;
	  }
	} else {
		childstate->UpdateFn(-1, state->_frm_mst_opt_node, state->GetFn());
    if (childstate->GetFn() < t->f_min || childstate->GetFn() > t->f_max || childstate->GetFn() >= U) {
	  // never do the below line.. it will disrupt starting from the min_path below.. so the optimization is not applicable for beam stack
	  // || (q->size() > w && q->bottom()->GetFn() <= childstate->GetFn())) {
	  delete childstate;
	  continue;
	}
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
    TspState* kt = temp->getState();
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
  for (int i = l; i < nodes - 1; i++) {
    while (!open[i]->empty()) {
      TspState* s = open[i]->popLast();
      qh->pop(s);
      delete s;
      CUR_MEM--;
    }
  }
  l = l - 1;
  
  while (l < nodes - 2 && !open[l]->empty()) {
	myPriorityQueue* temp = open[l];
    temp = temp->getNext();
    short minFound = 0;
	while (temp->getState() != NULL) {
	  TspState* node = temp->getState();
	  if (int(time(NULL) - start_time) > MAX_T) {
		return U;  
	  }
	  GenerateChildrenBS(node, open[l + 1], open, &minFound, U, qh, w);
	  
	  temp = temp->getNext();
	}
	if (l + 1 < nodes - 2) {
	  PruneLayer(open[l + 1], w, qh);
	}
	l = l + 1;
    beam_stack.push(new beam_stack_item(0, U, l + 2));
  }
  if (l == nodes - 2 && !open[l]->empty()) {
	TspState* node = open[l]->top();
	if (node->GetFn() < U) {
      U = node->GetFn();
	  fprintf(fp, "%d %lf %ld %ld %d\n", U, (OPT * 100.0) / U, CUR_MAX_MEM, NUM_NODES_EXPANDED, (int)(time(NULL) - start_time));
      fflush(fp);
	}
  }
  return U;
}

int Beam_Stack(int beam, FILE* fp) {
  time_t start_time = time(NULL);
  
  myPriorityQueue* open[nodes];
  for (int i = 0; i < nodes; i++) {
	open[i] = new myPriorityQueue(1);
  }
  HHashtable* qh = new HHashtable();
  
  TspState* s = CreateInitState();
  open[0]->push (s);
  qh->push(s);
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;

  int U = MAX_RES;
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
  
  for (int i = 0; i < nodes; i++) {
    while (!open[i]->empty()) {
	  TspState* s = open[i]->popLast();
      delete s;
    }
    delete open[i];  
  }
  qh->deleteAllEntries();
  delete qh;
  
  return U;
}
