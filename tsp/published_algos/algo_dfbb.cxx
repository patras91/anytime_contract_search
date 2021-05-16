
void GenerateChildren_DFBB (TspState * state, HHeap * q, HHashtable * cl, HHashtable * qh, int res) {
	NUM_NODES_EXPANDED++;
  for (int i = 1; i < nodes; i++) {
    if (state->IsChosen(i)) {
	  continue;
    }
    TspState *childstate = state->GetNewChild(i);
    List* clo = cl->getList(childstate);
    int index;
    bool found = 0;
    if (clo && (index = clo->getIndex(childstate)) != -1) {   
	  found = 1;
      TspState* oldState = clo->getState(index);
      if (oldState->GetGn() > childstate->GetGn()) {
		clo->popIndex(index);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
    if (childstate->GetFn() >= res) {
      delete childstate;
	  continue;
	}
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
	  TspState* oldState = qht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		q->pop(oldState);		
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
    if (childstate->GetFn() >= res) {
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
      //childstate->printTspzle();
		childstate->UpdateFn(-1, state->_frm_mst_opt_node, state->GetFn());
    if (childstate->GetFn() >= res) {
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
}

int DFBB (FILE* fp) {
  time_t start_time = time(NULL);
  time_t end_time;
  
  int res = MAX_RES;
  HHeap* q = new HHeap(2);
  HHashtable* qh = new HHashtable();
  HHashtable* clo = new HHashtable();
  
  TspState* s = CreateInitState();
  q->push (s);
  qh->push(s);
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;

  while (!q->empty ()) {
    TspState *n = (TspState *) q->top ();
    q->pop ();
    qh->pop(n);
    if (n->GetFn () >= res) {
	  delete n;
      CUR_MEM--;
	  continue;
	}
    end_time = time(NULL);
	if (n->IsGoalState()) {
      res = n->GetFn ();
      fprintf(fp, "%d %lf %ld %ld %d\n", res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(end_time - start_time));
	  fflush(fp);
	  delete n;
      CUR_MEM--;
	  continue;
	}
    if (int(end_time - start_time) > MAX_T) {
	  delete n;
	  break;
    }
    GenerateChildren_DFBB (n, q, clo, qh, res);
    clo->push(n);
  }
  fprintf(fp, "%d %lf %ld %ld %d\n", res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
  fflush(fp);
  
  q->deleteAllStates();
  delete q;
  qh->deleteAllEntries();
  delete qh;
  
  clo->deleteAllStates();
  delete clo;
  
  return res;
}
