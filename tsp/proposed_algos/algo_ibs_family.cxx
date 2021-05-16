/**
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
*/

int OPEN_BEAM_SIZE;
int RAW_BEAM_SIZE;

void IBSGenerateChildren(TspState *state, myPriorityQueue **open, myPriorityQueue **clo, myPriorityQueue **raw, int result, HHashtable* qh, HHashtable* ch) {
	NUM_NODES_EXPANDED++;
  myPriorityQueue* q = open[state->GetDepth() + 1];
  for (int i = 1; i < nodes; i++) {
    if (state->IsChosen(i)) {
	  continue;
	}
   	TspState *childstate = state->GetNewChild(i);
	int index;
	bool found = 0;
    List* cht = ch->getList(childstate);
	if (cht && (index = cht->getIndex(childstate)) != -1) {
	  found = 1;
      TspState* oldState = cht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		int d = oldState->GetDepth();
	    myPriorityQueue* p = clo[d]->getLink(oldState);
		clo[d]->deleteLink(p);
		cht->popIndex(index);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
	if (childstate->GetFn() >= result) {
      delete childstate;
      continue;
	}
		q->push(childstate);
		qh->push(childstate);
	  } else {
		delete childstate;
		childstate = NULL;
	  }
	}
	if (!found) {
    List* qht = qh->getList(childstate);
	if (qht && (index = qht->getIndex(childstate)) != -1) {
	  found = 1;
      TspState* oldState = qht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		int d = oldState->GetDepth();
	    myPriorityQueue* p = open[d]->getLink(oldState);
		open[d]->deleteLink(p);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
	if (childstate->GetFn() >= result) {
      delete childstate;
      qht->popIndex(index);
      continue;
	}
		q->push(childstate);
		qht->updateIndex(index, childstate);
	  } else {
		delete childstate;
		childstate = NULL;
	  }
	}
    }
	if (childstate == NULL) {
	  continue;
	}
	if (!found) {
		childstate->UpdateFn(-1, state->_frm_mst_opt_node, state->GetFn());
	if (childstate->GetFn() >= result) {
      delete childstate;
      continue;
	}
      q->push(childstate);
	  qh->push(childstate);
      CUR_MEM++;
      if (CUR_MAX_MEM < CUR_MEM) {
		CUR_MAX_MEM = CUR_MEM;
	  }
	}
	int cd = childstate->GetDepth();
    if (q->size() + clo[cd]->size() > OPEN_BEAM_SIZE) {
      TspState* s;
	  if (clo[cd]->empty()) {
		s = q->popLast();
		qh->pop(s);
        raw[cd]->push(s);
		if (raw[cd]->size() > RAW_BEAM_SIZE) {
		  s = raw[cd]->popLast();
		  delete s;
		  CUR_MEM--;
	    }
	  } else {
		s = clo[cd]->popLast();
		ch->pop(s);
		delete s;
		CUR_MEM--;
	  }
    }
  }
}

void MoveStepColumns(myPriorityQueue **open, myPriorityQueue **clo, myPriorityQueue **raw, int result, int STEP, HHashtable* qh, HHashtable* ch) {
    if (RAW_BEAM_SIZE < STEP) {
	  STEP = RAW_BEAM_SIZE;
    }
    OPEN_BEAM_SIZE += STEP;
    RAW_BEAM_SIZE -= STEP;
    for (int i = 0; i < nodes - 2; i++) {
	  for (int j = 0; j < STEP; j++) {
        if(raw[i]->empty ()) {
		  break;
		}
        TspState* s = (TspState *) raw[i]->top ();
        raw[i]->pop ();
        if (s->GetFn() >= result) {
		  delete s;
		  CUR_MEM--;
		  break;
		}
	int index;
	bool found = 0;
    List* cht = ch->getList(s);
	if (cht && (index = cht->getIndex(s)) != -1) {
	  found = 1;
      TspState* oldState = cht->getState(index);
	  if (oldState->GetGn() > s->GetGn()) {
		int d = oldState->GetDepth();
	    myPriorityQueue* p = clo[d]->getLink(oldState);
		clo[d]->deleteLink(p);
		cht->popIndex(index);
		delete oldState;
		CUR_MEM--;
		open[i]->push(s);
		qh->push(s);
	  } else {
		delete s;
		CUR_MEM--;
	  }
	}
	if (found) {
	  continue;
	}
    List* qht = qh->getList(s);
	if (qht && (index = qht->getIndex(s)) != -1) {
	  found = 1;
      TspState* oldState = qht->getState(index);
	  if (oldState->GetGn() > s->GetGn()) {
		int d = oldState->GetDepth();
	    myPriorityQueue* p = open[d]->getLink(oldState);
		open[d]->deleteLink(p);
		delete oldState;
		CUR_MEM--;
		open[i]->push(s);
		qht->updateIndex(index, s);
	  } else {
		delete s;
		CUR_MEM--;
	  }
	}
	if (found) {
	  continue;
	}
	open[i]->push(s);
	qh->push(s);
      }
    }
}

// Incremental Beam Search based on beam search.
int IBS(int BEAM, int STEP, FILE* fp) {
  time_t start_time = time(NULL);
  
  OPEN_BEAM_SIZE = STEP;
  RAW_BEAM_SIZE = BEAM - OPEN_BEAM_SIZE;
    
  myPriorityQueue **open = new myPriorityQueue*[nodes];
  myPriorityQueue **clo = new myPriorityQueue*[nodes];
  myPriorityQueue **raw = new myPriorityQueue*[nodes];
  for (int i = 0; i < nodes; i++) {
    open[i] = new myPriorityQueue (1);
    clo[i] = new myPriorityQueue (2);
    raw[i] = new myPriorityQueue (1);
  }  
  HHashtable* qh = new HHashtable();
  HHashtable* ch = new HHashtable();
  
  TspState* s = CreateInitState();
  open[0]->push(s);
  qh->push(s);
  int result = MAX_RES;
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;

  time_t end_time;
  while (1) {
    for (int i = 0; i < nodes - 2; i++) {
	  while (!open[i]->empty()) {
		TspState* s = open[i]->top();
	    if (s->GetFn() >= result) {
		  break;
	    }
	    end_time = time(NULL);
	    if (int(end_time - start_time) > MAX_T) {
		  break;
	    }
	    open[i]->pop();
	    qh->pop(s);
	    IBSGenerateChildren(s, open, clo, raw, result, qh, ch);
	    clo[i]->push(s);
	    ch->push(s);
	  }
	  if (!open[nodes - 2]->empty()) {
		TspState* s = open[nodes - 2]->top();
		if (s->GetFn() < result) {
		  result = s->GetFn();
		  fprintf(fp, "%d %d %lf %ld %ld %d\n", OPEN_BEAM_SIZE, result, (OPT * 100.0) / result, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
		  fflush(fp);
          while (!open[nodes - 2]->empty()) {
	        TspState* s = open[nodes - 2]->popLast();
	        qh->pop(s);
            delete s;
            CUR_MEM--;
          }
	    }
	  }
	  if (int(end_time - start_time) > MAX_T) {
	    break;
	  }
    }
    if (int(end_time - start_time) > MAX_T || RAW_BEAM_SIZE == 0) {
      break;
	}
	MoveStepColumns(open, clo, raw, result, STEP, qh, ch);
  }
  fprintf(fp, "%d %d %lf %ld %ld %d\n", OPEN_BEAM_SIZE, result, (OPT * 100.0) / result, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
  fflush(fp);
  
  for (int i = 0; i < nodes; i++) {
    while (!open[i]->empty()) {
	  TspState* s = open[i]->popLast();
      delete s;
    }
    delete open[i];  
    while (!clo[i]->empty()) {
	  TspState* s = clo[i]->popLast();
      delete s;
    }
    delete clo[i];  
    while (!raw[i]->empty()) {
	  TspState* s = raw[i]->popLast();
      delete s;
    }
    delete raw[i];  
  }
  qh->deleteAllEntries();
  delete qh;
  ch->deleteAllEntries();
  delete ch;
  
  return result;
}
/*
// expand one node at each level, loop until exhausted
int IBSF(int BEAM, int STEP, FILE* fp) {
  time_t start_time = time(NULL);
  
  OPEN_BEAM_SIZE = STEP;
  RAW_BEAM_SIZE = BEAM - OPEN_BEAM_SIZE;
    
  myPriorityQueue **open = new myPriorityQueue*[nodes];
  myPriorityQueue **clo = new myPriorityQueue*[nodes];
  myPriorityQueue **raw = new myPriorityQueue*[nodes];
  for (int i = 0; i < nodes; i++) {
    open[i] = new myPriorityQueue (1);
    clo[i] = new myPriorityQueue (2);
    raw[i] = new myPriorityQueue (1);
  }  
  HHashtable* qh = new HHashtable();
  HHashtable* ch = new HHashtable();
  
  TspState* s = CreateInitState();
  open[0]->push(s);
  qh->push(s);
  int result = MAX_RES;
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;

  time_t end_time;
  while (1) {
	bool nodeExpanded = 1;
	while (nodeExpanded) {
	  nodeExpanded = 0;
      for (int i = 0; i < result; i++) {
		  if (open[i]->empty()) {
		    continue;
		  }
		  TspState* s = open[i]->top();
	      if (s->GetFn() >= result) {
		    break;
	      }
	      if (s->IsGoalState()) {
		    result = s->GetFn();
		    fprintf(fp, "%d %d %lf %ld %ld %d\n", OPEN_BEAM_SIZE, result, (OPT * 100.0) / result, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
		    fflush(fp);
		    break;
	      }
	      end_time = time(NULL);
	      if (int(end_time - start_time) > MAX_T) {
		    break;
	      }
	      open[i]->pop();
	      qh->pop(s);
	      IBSGenerateChildren(s, open, clo, raw, result, qh, ch);
	      clo[i]->push(s);
	      ch->push(s);
	      nodeExpanded = 1;
      }
    }
    if (int(end_time - start_time) > MAX_T || RAW_BEAM_SIZE == 0) {
      break;
	}
	MoveStepColumns(open, clo, raw, result, STEP, qh, ch);
  }
  fprintf(fp, "%d %d %lf %ld %ld %d\n", OPEN_BEAM_SIZE, result, (OPT * 100.0) / result, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
  fflush(fp);
  
  for (int i = 0; i < nodes; i++) {
    while (!open[i]->empty()) {
	  TspState* s = open[i]->popLast();
      delete s;
    }
    delete open[i];  
    while (!clo[i]->empty()) {
	  TspState* s = clo[i]->popLast();
      delete s;
    }
    delete clo[i];  
    while (!raw[i]->empty()) {
	  TspState* s = raw[i]->popLast();
      delete s;
    }
    delete raw[i];  
  }
  qh->deleteAllEntries();
  delete qh;
  ch->deleteAllEntries();
  delete ch;
  
  return result;
}

// expand 1, 2, 3, ...
int IBSI(int BEAM, int STEP, FILE* fp) {
  time_t start_time = time(NULL);
  
  OPEN_BEAM_SIZE = STEP;
  RAW_BEAM_SIZE = BEAM - OPEN_BEAM_SIZE;
    
  myPriorityQueue **open = new myPriorityQueue*[nodes];
  myPriorityQueue **clo = new myPriorityQueue*[nodes];
  myPriorityQueue **raw = new myPriorityQueue*[nodes];
  for (int i = 0; i < nodes; i++) {
    open[i] = new myPriorityQueue (1);
    clo[i] = new myPriorityQueue (2);
    raw[i] = new myPriorityQueue (1);
  }  
  HHashtable* qh = new HHashtable();
  HHashtable* ch = new HHashtable();
  
  TspState* s = CreateInitState();
  open[0]->push(s);
  qh->push(s);
  int result = MAX_RES;
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;

  time_t end_time;
  while (1) {
	for (int cb = 1; cb <= OPEN_BEAM_SIZE; cb++) {
      for (int i = 0; i < result; i++) {
	    for (int j = 0; j < cb; j++) {
		  if (open[i]->empty()) {
		    break;
		  }
		  TspState* s = open[i]->top();
	      if (s->GetFn() >= result) {
		    break;
	      }
	      if (s->IsGoalState()) {
		    result = s->GetFn();
		    fprintf(fp, "%d %d %d %lf %ld %ld %d\n", cb, OPEN_BEAM_SIZE, result, (OPT * 100.0) / result, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
		    fflush(fp);
		    break;
	      }
	      end_time = time(NULL);
	      if (int(end_time - start_time) > MAX_T) {
		    break;
	      }
	      open[i]->pop();
	      qh->pop(s);
	      IBSGenerateChildren(s, open, clo, raw, result, qh, ch);
	      clo[i]->push(s);
	      ch->push(s);
	    }
      }
    }
    if (int(end_time - start_time) > MAX_T || RAW_BEAM_SIZE == 0) {
      break;
	}	
	MoveStepColumns(open, clo, raw, result, STEP, qh, ch);
  }
  fprintf(fp, "0 %d %d %lf %ld %ld %d\n", OPEN_BEAM_SIZE, result, (OPT * 100.0) / result, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
  fflush(fp);
  
  for (int i = 0; i < nodes; i++) {
    while (!open[i]->empty()) {
	  TspState* s = open[i]->popLast();
      delete s;
    }
    delete open[i];  
    while (!clo[i]->empty()) {
	  TspState* s = clo[i]->popLast();
      delete s;
    }
    delete clo[i];  
    while (!raw[i]->empty()) {
	  TspState* s = raw[i]->popLast();
      delete s;
    }
    delete raw[i];  
  }
  qh->deleteAllEntries();
  delete qh;
  ch->deleteAllEntries();
  delete ch;
  
  return result;
}

// expand 1, 2, 4, 7, 11, ... disturbed fibonacci.. weird.. but can be tried with correction.. fibonacci may be magical!
int IBSIM(int BEAM, int STEP, FILE* fp) {
  time_t start_time = time(NULL);
  
  OPEN_BEAM_SIZE = STEP;
  RAW_BEAM_SIZE = BEAM - OPEN_BEAM_SIZE;
    
  myPriorityQueue **open = new myPriorityQueue*[nodes];
  myPriorityQueue **clo = new myPriorityQueue*[nodes];
  myPriorityQueue **raw = new myPriorityQueue*[nodes];
  for (int i = 0; i < nodes; i++) {
    open[i] = new myPriorityQueue (1);
    clo[i] = new myPriorityQueue (2);
    raw[i] = new myPriorityQueue (1);
  }  
  HHashtable* qh = new HHashtable();
  HHashtable* ch = new HHashtable();
  
  TspState* s = CreateInitState();
  open[0]->push(s);
  qh->push(s);
  int result = MAX_RES;
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;

  time_t end_time;
  while (1) {
	int d = 0;
	for (int cb = 1; cb <= OPEN_BEAM_SIZE;) {
      for (int i = 0; i < result; i++) {
	    for (int j = 0; j < cb; j++) {
		  if (open[i]->empty()) {
		    break;
		  }
		  TspState* s = open[i]->top();
	      if (s->GetFn() >= result) {
		    break;
	      }
	      if (s->IsGoalState()) {
		    result = s->GetFn();
		    fprintf(fp, "%d %d %d %lf %ld %ld %d\n", cb, OPEN_BEAM_SIZE, result, (OPT * 100.0) / result, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
		    fflush(fp);
		    break;
	      }
	      end_time = time(NULL);
	      if (int(end_time - start_time) > MAX_T) {
		    break;
	      }
	      open[i]->pop();
	      qh->pop(s);
	      IBSGenerateChildren(s, open, clo, raw, result, qh, ch);
	      clo[i]->push(s);
	      ch->push(s);
	    }
      }
      d++;
      cb += d;
      if (cb > OPEN_BEAM_SIZE && cb - d != OPEN_BEAM_SIZE) {
		cb = OPEN_BEAM_SIZE;
	  }
    }
    if (int(end_time - start_time) > MAX_T || RAW_BEAM_SIZE == 0) {
      break;
	}
	MoveStepColumns(open, clo, raw, result, STEP, qh, ch);
  }
  fprintf(fp, "0 %d %d %lf %ld %ld %d\n", OPEN_BEAM_SIZE, result, (OPT * 100.0) / result, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(time(NULL) - start_time));
  fflush(fp);
  
  for (int i = 0; i < nodes; i++) {
    while (!open[i]->empty()) {
	  TspState* s = open[i]->popLast();
      delete s;
    }
    delete open[i];  
    while (!clo[i]->empty()) {
	  TspState* s = clo[i]->popLast();
      delete s;
    }
    delete clo[i];  
    while (!raw[i]->empty()) {
	  TspState* s = raw[i]->popLast();
      delete s;
    }
    delete raw[i];  
  }
  qh->deleteAllEntries();
  delete qh;
  ch->deleteAllEntries();
  delete ch;
  
  return result;
}
*/
