
void GenerateChildrenB(TspState* state, myPriorityQueue* open, myPriorityQueue* child, int res, int beam, myhashtable* ch) {
	NUM_NODES_EXPANDED++;
  for (int i = 1; i < nodes; i++) {
    if (state->IsChosen(i)) {
	  continue;
	}
    TspState *childstate = state->GetNewChild(i);
    int index;
    List* cht = ch->getList(childstate);
    if (cht && (index = cht->getIndex(childstate)) != -1) {
      TspState* oldState = cht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
        myPriorityQueue* p = child->getLink(oldState);
	    child->deleteLink(p);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
	if (childstate->GetFn() >= res || (child->size() == beam && childstate->GetFn() >= child->bottom()->GetFn())) {
      delete childstate;
      cht->popIndex(index);
	  continue;
	}
		child->push(childstate);
		cht->updateIndex(index, childstate);
	  } else {
		delete childstate;
	  }
	} else {
		childstate->UpdateFn(-1, state->_frm_mst_opt_node, state->GetFn());
	if (childstate->GetFn() >= res || (child->size() == beam && childstate->GetFn() >= child->bottom()->GetFn())) {
      delete childstate;
	  continue;
	}
      child->push (childstate);
      if (cht) {
		cht->push(childstate);
	  } else {
        ch->push (childstate);
      }
      CUR_MEM++;
      if (CUR_MAX_MEM < CUR_MEM) {
		CUR_MAX_MEM = CUR_MEM;
	  }
      if (child->size() > beam) {
	    TspState* s = child->popLast();
	    ch->pop(s);
	    delete s;
	    CUR_MEM--;
	  }
    }
  }
}

int BeamR(TspState* s, int beam, FILE* fp, int res, time_t start_time) {
  myPriorityQueue* open = new myPriorityQueue(0);
  myPriorityQueue* child = new myPriorityQueue(0);
  //myhashtable* qh = new myhashtable();
  myhashtable* ch = new myhashtable();
  //myhashtable* clo = new myhashtable();  
  
  open->push (s);
  int openl = s->GetDepth();
  
  time_t end_time;
  while (!open->empty()) {
	if (openl == nodes - 2) {
	  break;
	}
	for (int j = 0; j < beam; j++) {
	  if (open->empty()) {
		break;
	  }
	  TspState* s = open->top();
	  open->pop();
	  if (s->GetFn() >= res) {
		delete s;
	    CUR_MEM--;
		break;
	  }
	  if (child->size() == beam && s->GetFn() >= child->bottom()->GetFn()) {
		break;
	  }
	  end_time = time(NULL);
	  if (int(end_time - start_time) > MAX_T) {
		break;
	  }
	  GenerateChildrenB(s, open, child, res, beam, ch);
	  delete s;
      CUR_MEM--;
	}
	if (int(end_time - start_time) > MAX_T) {
	  break;
	}
	//child->print();
    while (!open->empty()) {
	  TspState* s = open->popLast();
      delete s;
      CUR_MEM--;
    }
	myPriorityQueue* t = open;
	open = child;
	child = t;/*
	myhashtable* th = qh;
	qh = ch;
	ch = th;*/
	ch->resetAllEntries();
	openl++;
  }
  if (openl == nodes - 2 && !open->empty()) {
	s = open->top();
	if (s->GetFn() < res) {
	  res = s->GetFn();
      fprintf(fp, "%d %d %lf %ld %ld %d\n", beam, res, (OPT * 100.0) / res, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(end_time - start_time));
      fflush(fp);
	}
  }
  
    while (!open->empty()) {
	  TspState* s = open->popLast();
      delete s;
      CUR_MEM--;
    }
  delete open;
  
    while (!child->empty()) {
	  TspState* s = child->popLast();
      delete s;
      CUR_MEM--;
    }
  delete child;
  ch->deleteAllEntries();
  delete ch;
  
  return res;
}

int Beam(int beam, FILE* fp) {
  time_t start_time = time(NULL);
  NUM_NODES_EXPANDED = 0;
  CUR_MAX_MEM = 1;
  CUR_MEM = 1;
  return BeamR(CreateInitState(), beam, fp, MAX_RES, start_time);
}

int ItB(int beam, int step, FILE* fp) {
  time_t start_time = time(NULL);
  int U = MAX_RES;
  int i;
  NUM_NODES_EXPANDED = 0;
  CUR_MAX_MEM = 1;
  for (i = step; i <= beam; i += step) {
    CUR_MEM = 1;
	U = BeamR(CreateInitState(), i, fp, U, start_time);
	if (int(time(NULL) - start_time) > MAX_T) {
	  break;
	}
  }
  fprintf(fp, "%d %d %lf %ld %ld %d\n", i, U, (OPT * 100.0) / U, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
  fflush(fp);
  return U;
}
