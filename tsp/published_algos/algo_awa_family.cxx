
void AWGenerateChildrenGraph(TspState * state, HHeap * q, HHeap * sus, HHashtable * cl, int res, HHashtable * qh, HHashtable * sush) {
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
    List* susht = sush->getList(childstate);
	if (susht && (index = susht->getIndex(childstate)) != -1) {
	  found = 1;
      TspState* oldState = susht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		sus->pop(oldState);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;   
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  susht->popIndex(index);
	  continue;
	}
		sus->push(childstate);
		susht->updateIndex(index, childstate);
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

int AW(FILE* fp, int mode) {
  time_t start_time;
  time_t end_time;
  start_time = time(NULL);
  
  HHeap *q = new HHeap(0);
  HHashtable* qh = new HHashtable();
  //HHeap *clo = new HHeap(-1);
  HHashtable* clo = new HHashtable();
  HHeap *sus = new HHeap(0);
  HHashtable* sush = new HHashtable();
  
  int w = 0;
  TspState* s = CreateInitState();
  q->push (s);
  qh->push(s);
  int res = MAX_RES;
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;

  while(1) {
    int topLevel = -1;
    bool goalFound = 0;
    while (!q->empty ()) {
	  TspState *s = q->top ();
	  q->pop ();
	  qh->pop(s);
	  if (s->GetFn () >= res) {
		delete s;
		CUR_MEM--;
	    break;
	  }
	  if (s->GetDepth() > topLevel) {
        topLevel = s->GetDepth();
	  } else if (s->GetDepth() <= topLevel - w) {
        sus->push (s);
        sush->push(s);
	    continue;
	  }
      end_time = time(NULL);
	  if (s->IsGoalState ()) {
        res = s->GetFn ();
        fprintf(fp, "%d %d %lf %ld %ld %d\n", w, res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(end_time - start_time)); 
        fflush(fp);
        delete s;
		CUR_MEM--;
        goalFound = 1;
	    break;
	  }
      if (int(end_time - start_time) > MAX_T) {
		delete s;
	    break;
	  }
	  AWGenerateChildrenGraph(s, q, sus, clo, res, qh, sush);
	  clo->push(s);
	}
      
    if (sus->empty () || int(time(NULL) - start_time) > MAX_T) {
	  break;
	} else {
	  switch(mode) {
		case 0:  // progressive
		  w++;
		  break;
		case 1:  // scaling
		  if (goalFound) {
			w = 0;
		  } else {
			w++;
		  }
		  break;
		case 2:  // adaptive
		  if (!goalFound) {
			w++;
		  }
		  break;
	  }
	  /*
      while(!q->empty ()) {
        TspState *s = (TspState *) q->top ();
        q->pop ();
        delete s;
      }*/
      CUR_MEM -= q->Size();
      q->deleteAllStates();
      qh->resetAllEntries();
      HHeap* t = q;
	  q = sus;
	  sus = t;
	  HHashtable* th = qh;
	  qh = sush;
	  sush = th;
	}
  }
  fprintf(fp, "%d %d %lf %ld %ld %d\n", w, res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time)); 
  fflush(fp);
/*
  while(!sus->empty ()) {
    TspState *s = (TspState *) sus->top ();
    sus->pop ();
    delete s;
  }*/
  sus->deleteAllStates();
  delete sus;
  sush->deleteAllEntries();
  delete sush;
/*
  while(!q->empty ()) {
    TspState *s = (TspState *) q->top ();
    q->pop ();
    delete s;
  }*/
  q->deleteAllStates();
  delete q;
  qh->deleteAllEntries();
  delete qh;
/*
  while(!clo->empty ()) {
    TspState *s = (TspState *) clo->top ();
    clo->pop ();
    delete s;
  }*/
  clo->deleteAllStates();
  delete clo;
  
  return res;
}

int AWA(FILE* fp) {
  return AW(fp, 0);
}

int AWAS(FILE* fp) {
  return AW(fp, 1);
}
