/*
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com),
 * Sandip Aine
*/

//#include <queue>
//#define PriorityQueue priority_queue<PuzState*, vector<PuzState*>, bool (*)(PuzState*, PuzState*)>

void GenerateChildren (PuzState* state, HHeap* q, HHashtable * cl, HHashtable * qh) {
  NUM_NODES_EXPANDED++;
  for (int i = 0; i < 4; i++) {
    if (!state->IsChildPossible (i)) {
	  continue;
	}
    PuzState *childstate = state->getChild(i, state->GetHn());  
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
      //childstate->print();
      q->push (childstate);
      if (qht) {
		qht->push(childstate);
	  } else {
        qh->push (childstate);
      }
      CUR_MEM++;
      CUR_MAX_MEM++;
    }
  }
}

double AStar(FILE* fp) {
  time_t start_time = time(NULL);
  HHeap *q = new HHeap(0);
  HHashtable* qh = new HHashtable();
  //HHeap *clo = new HHeap(-1);
  HHashtable* clo = new HHashtable();
  PuzState* s = CreateInitState();
  q->push (s);
  qh->push(s);
  int res;
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;
  
  while (!q->empty ()) {
    s = (PuzState *) q->top ();
    q->pop ();
    qh->pop(s);
    if (s->IsGoalState ()) {
   	  res = s->GetFn ();
	  fprintf(fp, "%d %ld %ld %d\n", res, CUR_MAX_MEM, NUM_NODES_EXPANDED, (int)(time(NULL) - start_time));
	  fflush(fp);
   	  delete s;
   	  break;
   	}
    GenerateChildren (s, q, clo, qh);
    clo->push(s);
  }
  
  q->deleteAllStates();
  delete q;
  qh->deleteAllEntries();
  delete qh;
  
  clo->deleteAllStates();
  delete clo;
  
  return res;
}
