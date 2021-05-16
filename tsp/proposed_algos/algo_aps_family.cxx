
TspState * APGenerateChildrenGraphHelper(int i, TspState * state, myPriorityQueue * CP, HHashtable * cl, HHeap * sus, myPriorityQueue * OP, int res, HHashtable *qh, HHashtable *ch, HHashtable *sush) {
    if (state->IsChosen(i)) {
	  return NULL;
	}
    TspState *childstate = state->GetNewChild(i); 
    List* clo = cl->getList(childstate);
    int index;
    if (clo && (index = clo->getIndex(childstate)) != -1) {
      TspState* oldState = clo->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		clo->popIndex(index);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  return NULL;
	}
		return childstate;
	  }
	  delete childstate;
	  return NULL;
	}
    List* susht = sush->getList(childstate);
	if (susht && (index = susht->getIndex(childstate)) != -1) {
      TspState* oldState = susht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		sus->pop(oldState);
        susht->popIndex(index);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  return NULL;
	}
	    return childstate;
	  }
	  delete childstate;
      return NULL;
	}
    List* qht = qh->getList(childstate);
    if (qht && (index = qht->getIndex(childstate)) != -1) {
      TspState* oldState = qht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
	    myPriorityQueue* p = OP->getLink(oldState);
        OP->deleteLink(p);	
        qht->popIndex(index);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  return NULL;
	}
	    return childstate;
	  }
	  delete childstate;
	  return NULL;
	}
    List* cht = ch->getList(childstate);
    if (cht && (index = cht->getIndex(childstate)) != -1) {
      TspState* oldState = cht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
	    myPriorityQueue* p = CP->getLink(oldState);
        CP->deleteLink(p);
        cht->popIndex(index);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  return NULL;
	}
	    return childstate;
	  }
	  delete childstate;
	  return NULL;
	}
		childstate->UpdateFn(-1, state->_frm_mst_opt_node, state->GetFn());
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  return NULL;
	}
    CUR_MEM++;
    if (CUR_MAX_MEM < CUR_MEM) {
	  CUR_MAX_MEM = CUR_MEM;
	}  
    return childstate;
}

void APGenerateChildrenGraphK(TspState * state, myPriorityQueue * CP, HHashtable * cl, HHeap * sus, myPriorityQueue * OP, int res, int K, HHashtable *qh, HHashtable *ch, HHashtable *sush) {
	NUM_NODES_EXPANDED++;
  myPriorityQueue* q = CP;
  for (int i = 1; i < nodes; i++) {
	TspState *childstate = APGenerateChildrenGraphHelper(i, state, CP, cl, sus, OP, res, qh, ch, sush);
    if (childstate) {
	 if (q->size() < K || childstate->GetFn() < q->bottom()->GetFn()) {
      q->push (childstate);
      ch->push(childstate);  // note q = cp!
      if (q->size() > K) {
	    TspState* s = q->popLast();
	    ch->pop(s);
	    sus->push(s);
	    sush->push(s);
	  }
     } else {
	   sus->push(childstate);
	   sush->push(childstate);
	 }
    }
  }
}
/*
void APGenerateChildrenGraph(TspState * state, myPriorityQueue * CP, HHeap * clo, HHeap * sus, myPriorityQueue * OP, int res) {
  for (int i = 0; i < 4; i++) {
	TspState *childstate = APGenerateChildrenGraphHelper(i, state, CP, clo, sus, OP, res);
    if (childstate) {
      CP->push (childstate);
    }
  }
}
*/
int ExpandK(HHeap* t, int k, int res, myPriorityQueue * CP, HHashtable* cl, HHeap * sus, myPriorityQueue * OP, time_t start_time, FILE* fp, HHashtable *qh, HHashtable *ch, HHashtable *sush) {
	int n = k;
	while (n) {
	  if (t->empty()) {
		break;
	  }
	  TspState *s = t->top ();
	  if (CP->size() == k && s->GetFn() >= CP->bottom()->GetFn()) {
		break;
	  }
	  t->pop ();
	  sush->pop(s);
	  if (s->GetFn () >= res) {
		delete s;
		CUR_MEM--;
	    break;
	  }
      time_t end_time = time(NULL);
	  if (s->IsGoalState ()) {
        res = s->GetFn ();
        fprintf(fp, "%d %d %lf %ld %ld %d\n", k, res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(end_time - start_time)); 
        fflush(fp); 
        delete s;
        CUR_MEM--;
	    break;
	  }
      if (int(end_time - start_time) > MAX_T) {
	    break;
	  }
	  
	  APGenerateChildrenGraphK(s, CP, cl, sus, OP, res, k, qh, ch, sush);
	  cl->push(s);
	  
	  n--;
	}
	/*
    while (CP->size() > k) {
	  TspState* s = CP->popLast();
	  sus->push(s);
	}*/
	if (!t->empty() && t->top()->GetFn() >= res) {
	  CUR_MEM -= t->Size();
	  t->deleteAllStates();
	  sush->resetAllEntries();
	}
	return res;
}

int ExpandK(myPriorityQueue* t, int k, int res, myPriorityQueue * CP, HHashtable * cl, HHeap * sus, myPriorityQueue * OP, time_t start_time, FILE* fp, HHashtable *qh, HHashtable *ch, HHashtable *sush) {
	int n = k;
	while (n) {
	  if (t->empty()) {
		break;
	  }
	  TspState *s = t->top ();
	  if (CP->size() == k && s->GetFn() >= CP->bottom()->GetFn()) {
		while (!t->empty()) {
		  TspState *s = t->popLast();
		  qh->pop(s);
	      if (s->GetFn () >= res) {
		    delete s;
		    CUR_MEM--;
		  } else {
		    sus->push(s);
		    sush->push(s);
	      }
		}
		break;
	  }
	  t->pop ();
	  qh->pop(s);
	  if (s->GetFn () >= res) {
		delete s;
		CUR_MEM--;
	    break;
	  }
      time_t end_time = time(NULL);
	  if (s->IsGoalState ()) {
        res = s->GetFn ();
        fprintf(fp, "%d %d %lf %ld %ld %d\n", k, res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(end_time - start_time)); 
        fflush(fp); 
        delete s;
        CUR_MEM--;
	    break;
	  }
      if (int(end_time - start_time) > MAX_T) {
	    break;
	  }
	  
	  APGenerateChildrenGraphK(s, CP, cl, sus, OP, res, k, qh, ch, sush);
	  cl->push(s);
	  
	  n--;
	}
	if (!t->empty() && t->top()->GetFn() >= res) {
      while (!t->empty()) {
        TspState* s = t->popLast();
        delete s;
        CUR_MEM--;
      }
      qh->resetAllEntries();
	}
	return res;
}

//Anytime Progressive Pack Search
int AP(FILE* fp, int mode, int I, int F, int BOUND) {
  time_t start_time = time(NULL);
  
  HHeap* sus = new HHeap(0);
  HHashtable* sush = new HHashtable();
  
  //HHeap *clo = new HHeap(-1);
  HHashtable* clo = new HHashtable();
  
  myPriorityQueue* OP = new myPriorityQueue(0);
  HHashtable* qh = new HHashtable();
  
  myPriorityQueue* CP = new myPriorityQueue(0);
  HHashtable* ch = new HHashtable();
  
  TspState* s = CreateInitState();
  sus->push (s);
  sush->push(s);
  int res = MAX_RES;
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;
  int k = I;
  
  while(!sus->empty()) {
	int old_res = res;
	res = ExpandK(sus, k, res, CP, clo, sus, OP, start_time, fp, qh, ch, sush);
	while (!CP->empty()) {
      myPriorityQueue* t = OP;
      OP = CP;
      CP = t;
      HHashtable* th = qh;
      qh = ch;
      ch = th;
	  res = ExpandK(OP, k, res, CP, clo, sus, OP, start_time, fp, qh, ch, sush);
      if (int(time(NULL) - start_time) > MAX_T) {
	    break;
	  }
	}
    if (int(time(NULL) - start_time) > MAX_T) {
	  break;
	}
	switch(mode) {
	  case 0:  // progressive
	    k += F;
	    break;
	  case 1:  // scaling
	    if (res < old_res) {
		  k = I;
		} else {
	      k += F;
		}
	    break;
	  case 2:  // adaptive
	    if (res == old_res) {
	      k += F;
		}
	    break;
	  case 3:  // multi progressive
	    if (k < BOUND) {
	      k *= F;
	    }
	    break;
	  case 4:  // multi scaling
	    if (res < old_res) {
		  k = I;
		} else if (k < BOUND) {
	      k *= F;
		}
	    break;
	  case 5:  // multi adaptive
	    if (res == old_res && k < BOUND) {
	      k *= F;
		}
	    break;
	  default:  // constant
	    break;
	}
  }
  fprintf(fp, "%d %d %lf %ld %ld %d\n", k, res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time)); 
  fflush(fp); 

  sus->deleteAllStates();
  delete sus;
  sush->deleteAllEntries();
  delete sush;

  clo->deleteAllStates();
  delete clo;

  while (!OP->empty()) {
    TspState* s = OP->popLast();
    delete s;
  }
  delete OP;
  qh->deleteAllEntries();
  delete qh;

  while (!CP->empty()) {
    TspState* s = CP->popLast();
    delete s;
  }
  delete CP;
  ch->deleteAllEntries();
  delete ch;
  
  return res;
}

int API(FILE* fp, int k) {
  return AP(fp, -1, k, 0, 0);
}

int APP(FILE* fp) {
  return AP(fp, 0, 1, 1, 0);
}
	
int APS(FILE* fp) {
  return AP(fp, 1, 1, 1, 0);
}
	
int APA(FILE* fp) {
  return AP(fp, 2, 1, 1, 0);
}
	
int APGS(FILE* fp, int BASE, int MUL, int PEAK) {
  return AP(fp, 4, BASE, MUL, PEAK);
}
