
void GenerateChildrenB(PuzState* state, myPriorityQueue* q, myPriorityQueue* open[], int res, int beam, HHashtable* qh) {
	NUM_NODES_EXPANDED++;
  for (int i = 0; i < 4; i++) {
    if (!state->IsChildPossible(i)) {
	  continue;
	}
    PuzState *childstate = state->getChild(i, state->GetHn());
	if (childstate->GetFn() >= res || (q->size() == beam && childstate->GetFn() >= q->bottom()->GetFn())) {
      delete childstate;
	  continue;
	}
    int index;
    List* qht = qh->getList(childstate);
    if (qht && (index = qht->getIndex(childstate)) != -1) {
      PuzState* oldState = qht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		myPriorityQueue* p = open[oldState->GetDepth()]->getLink(childstate);
		open[oldState->GetDepth()]->deleteLink(p);		
		delete oldState;
		q->push(childstate);
		qht->updateIndex(index, childstate);
        if (q->size() > beam) {
	      PuzState* s = q->popLast();
	      qh->pop(s);
	      delete s;
	    }
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
      if (q->size() > beam) {
	    PuzState* s = q->popLast();
	    qh->pop(s);
	    delete s;
	    CUR_MEM--;
	  }
    }
  }
}

int BeamR(int beam, FILE* fp, int res, time_t start_time) {
  int NUM_QS = res;
  myPriorityQueue* open[NUM_QS];
  for (int i = 0; i < NUM_QS; i++) {
	open[i] = new myPriorityQueue(1);
  }
  HHashtable* qh = new HHashtable();
  
  PuzState* s = CreateInitState();
  open[0]->push (s);
  qh->push(s);
  CUR_MEM = 1;

  time_t end_time;
  for (int i = 0; i < res; i++) {
	myPriorityQueue* t = open[i];
	for (int j = 0; j < beam; j++) {
	  PuzState* s = t->next->s;
	  if (!s) {
		break;
	  }
	  if (s->GetFn() >= res) {
		break;
	  }
	  if (open[i + 1]->size() == beam && s->GetFn() >= open[i + 1]->bottom()->GetFn()) {
		break;
	  }
	  if (s->IsGoalState()) {
		res = s->GetFn();
        fprintf(fp, "%d %d %lf %ld %ld %d\n", beam, res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
        fflush(fp);
		break;
	  }
	  end_time = time(NULL);
	  if (int(end_time - start_time) > MAX_T) {
		break;
	  }
	  GenerateChildrenB(s, open[i + 1], open, res, beam, qh);
	  t = t->next;
	}
	if (int(end_time - start_time) > MAX_T) {
	  break;
	}
  }
  
  for (int i = 0; i < NUM_QS; i++) {
    while (!open[i]->empty()) {
	  PuzState* s = open[i]->popLast();
      delete s;
    }
    delete open[i];  
  }
  qh->deleteAllEntries();
  delete qh;
  
  return res;
}

int Beam(int beam, FILE* fp) {
  time_t start_time = time(NULL);
  NUM_NODES_EXPANDED = 0;
  CUR_MAX_MEM = 1;
  return BeamR(beam, fp, MAX_D, start_time);
}

int ItB(int beam, int step, FILE* fp) {
  time_t start_time = time(NULL);
  int U = MAX_D;
  int i;
  NUM_NODES_EXPANDED = 0;
  CUR_MAX_MEM = 1;
  for (i = step; i <= beam; i += step) {
	U = BeamR(i, fp, U, start_time);
	if (int(time(NULL) - start_time) > MAX_T) {
	  break;
	}
  }
  fprintf(fp, "%d %d %lf %ld %ld %d\n", i, U, (OPT * 100.0) / U, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
  fflush(fp);
  return U;
}
