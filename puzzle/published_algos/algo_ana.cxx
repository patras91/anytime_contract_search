/**
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
*/

void GenerateChildrenGraphANA(PuzState * state, HHeap * q, HHashtable * cl, HHashtable * qh) {
	NUM_NODES_EXPANDED++;
  for (int i = 0; i < 4; i++) {
    if (!state->IsChildPossible(i)) {
	  continue;
    }
    PuzState *childstate = state->getChild(i, state->GetHn());
    if (childstate->GetFn() >= result_ana) {
	  delete childstate;
	  continue;
	}
    List* clo = cl->getList(childstate);
    int index;
    bool found = 0;
    if (clo && (index = clo->getIndex(childstate)) != -1) {   
	  found = 1;
      PuzState* oldState = clo->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		clo->popIndex(index);
		delete oldState;
		q->push(childstate);
		qh->push(childstate);
	  } else {
		delete childstate;
	  }
	}
	if (found) {
	  continue;
	}
    List* qht = qh->getList(childstate);
    if (qht && (index = qht->getIndex(childstate)) != -1) {
      PuzState* oldState = qht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		q->pop(oldState);		
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
}

void ImprovePathANA(HHeap *q, HHashtable *clo, FILE* fp, time_t start_time, HHashtable * qh) {
  while(!q->empty()) {
	PuzState* s = q->top();
	q->pop();
	qh->pop(s);
	if (s->GetFn() >= result_ana) {
	  delete s;
	  CUR_MEM--;
	  continue;
	}
	time_t end_time = time(NULL);
	if (s->IsGoalState()) {
	  result_ana = s->GetFn();
	  fprintf(fp, "%d %lf %ld %ld %d\n", result_ana, OPT * 100.0 / result_ana, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(end_time - start_time));
	  fflush(fp);
	  delete s;
      CUR_MEM--;
	  return;
	}
	if (int(end_time - start_time) > MAX_T) {
	  delete s;
	  return;
	}
	GenerateChildrenGraphANA(s, q, clo, qh);
	clo->push(s);
  }
}

int ANA(FILE* fp) {
  time_t start_time = time(NULL);
  
  result_ana = MAX_D;
  HHeap *q = new HHeap(1);
  HHashtable* qh = new HHashtable();
  //HHeap *clo = new HHeap(-1);
  HHashtable* clo = new HHashtable();
  
  PuzState* s = CreateInitState();
  q->push (s);
  qh->push(s);
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;
  
  while(!q->empty()) {
    ImprovePathANA(q, clo, fp, start_time, qh);
	if (int(time(NULL) - start_time) > MAX_T) {
	  break;
	}
	q->Heapify();
  }
  
  fprintf(fp, "%d %lf %ld %ld %d\n", result_ana, OPT * 100.0 / result_ana, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
  fflush(fp);
  
  q->deleteAllStates();
  delete q;
  qh->deleteAllEntries();
  delete qh;
  
  clo->deleteAllStates();
  delete clo;
  
  return result_ana;
}
