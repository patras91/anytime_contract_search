
void ACGenerateChildrenGraph(TspState * state, Heap ** open, HHashtable* cl, int res, HHashtable* qh) {
	NUM_NODES_EXPANDED++;
  Heap* q = open[state->GetDepth() + 1];
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
		open[oldState->GetDepth()]->pop(oldState);		
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

int AC(FILE* fp, int mode, int I, int F, int BOUND) {
  time_t start_time = time(NULL);
  time_t end_time;
  
  Heap** q = new Heap*[nodes];
  for (int i = 0; i < nodes; i++) {
	q[i] = new Heap(0);
  }
  HHashtable* qh = new HHashtable();
  //Heap *clo = new Heap(-1);
  HHashtable* clo = new HHashtable();
  
  TspState* s = CreateInitState();
  q[0]->push (s);
  qh->push(s);
  int res = MAX_RES;
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;
  
  int iter = I;
  bool oneExpanded = 1;
  while(oneExpanded) {
    oneExpanded = 0;
	for (int i = 0; i < nodes - 2; i++) {
    for (int j = 0; j < iter; j++) {
	  if (q[i]->empty()) {
		break;
	  }
	  TspState *s = q[i]->top ();
	  q[i]->pop ();
	  qh->pop(s);
	  if (s->GetFn () >= res) {
		delete s;
		CUR_MEM--;
	    break;
	  }
      end_time = time(NULL);
      if (int(end_time - start_time) > MAX_T) {
	    break;
	  }
	  ACGenerateChildrenGraph(s, q, clo, res, qh);
	  clo->push(s);
	  oneExpanded = 1;
    }
      if (int(end_time - start_time) > MAX_T) {
	    break;
	  }
	}
    bool goalFound = 0;
	  //if (!q[nodes - 2]->empty()) {
		TspState* s = q[nodes - 2]->top();  // all nodes at this level are goal nodes and there will at-least be one node at this stage in this list.
		if (s->GetFn() < res) {
          res = s->GetFn ();
          fprintf(fp, "%d %d %lf %ld %ld %d\n", iter, res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time)); 
          fflush(fp);
          goalFound = 1;
	    }
	  //}
	switch(mode) {
	  case 0:  // progressive
	    iter += F;
	    break;
	  case 1:  // scaling
	    if (goalFound) {
		  iter = I;
		} else {
	      iter += F;
		}
	    break;
	  case 2:  // adaptive
	    if (!goalFound) {
	      iter += F;
		}
	    break;
	  case 3:  // multi progressive
	    if (iter < BOUND) {
	      iter *= F;
	    }
	    break;
	  case 4:  // multi scaling
	    if (goalFound) {
		  iter = I;
		} else if (iter < BOUND) {
	      iter *= F;
		}
	    break;
	  case 5:  // multi adaptive
	    if (!goalFound && iter < BOUND) {
	      iter *= F;
		}
	    break;
	  default:  // constant
	    break;
	}
    if (int(end_time - start_time) > MAX_T) {
	  break;
	}
  }
  fprintf(fp, "%d %d %lf %ld %ld %d\n", iter, res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time)); 
  fflush(fp);

  for (int i = 0; i < nodes; i++) {
    q[i]->deleteAllStates();
    delete q[i];
  }
  delete q;
  qh->deleteAllEntries();
  delete qh;

  clo->deleteAllStates();
  delete clo;
  
  return res;
}

int ACI(int I, FILE* fp) {
  return AC(fp, -1, I, 0, 0);
}

int ACP(FILE* fp) {
  return AC(fp, 0, 1, 1, 0);
}

int ACS(FILE* fp) {
  return AC(fp, 1, 1, 1, 0);
}

int ACA(FILE* fp) {
  return AC(fp, 2, 1, 1, 0);
}
