/**
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
*/
/*
int ABSR(TspState* start, int beam, time_t start_time, int result) {   
  myPriorityQueue *open = new myPriorityQueue(1);
  myPriorityQueue *open1 = new myPriorityQueue(1);

  open->push(start);
  time_t end_time;
  for (int level = 1; level < nodes - 2; level++) {
    while(!open->empty()) {
	  TspState* s = open->top();
	  open->pop();
	  GenerateChildren(s, open1, beam, result);
	  delete s;
	  end_time = time(NULL);
      if (int(end_time - start_time) > MAX_T) {
        break;
	  }
    }
    myPriorityQueue *t = open;
    open = open1;
    open1 = t;
    if (int(end_time - start_time) > MAX_T) {
      break;
	}
  }
  
  TspState* s = open->top();
  if (s && s->IsGoalState()) {
    result = s->GetFn();
  }
  
  while (!open->empty()) {
    s = open->top();
    open->pop();
    delete s;
  }
  delete open;
  
  while (!open1->empty()) {
    s = open1->top();
    open1->pop();
    delete s;
  }
  delete open1;
  
  return result;
}
*
int ABSR(TspState* start, int beam, time_t start_time, int res, FILE* fp) {
  int NUM_QS = nodes - 2;
  myPriorityQueue* open[NUM_QS];
  for (int i = 0; i < NUM_QS; i++) {
	open[i] = new myPriorityQueue(1);
  }
  open[0]->push(start);

  time_t end_time;
  for (int i = 0; i < nodes - 2; i++) {
	myPriorityQueue* t = open[i];
	for (int j = 0; j < beam; j++) {
	  TspState* s = t->next->s;
	  if (!s) {
		break;
	  }
	  if (s->GetFn() >= res) {
		break;
	  }
	  if (s->IsGoalState()) {
		res = s->GetFn();
		break;
	  }
	  end_time = time(NULL);
	  if (int(end_time - start_time) > MAX_T) {
		break;
	  }
	  GenerateChildrenB(s, open[i + 1], res, beam);
	  t = t->next;
	}
	if (int(end_time - start_time) > MAX_T) {
	  break;
	}
  }
  fprintf(fp, "%d %d %lf %d\n", beam, res, (OPT * 100.0) / res, int(time(NULL) - start_time));
  fflush(fp);
  
  for (int i = 0; i < NUM_QS; i++) {
    while (!open[i]->empty()) {
	  TspState* s = open[i]->popLast();
      delete s;
    }
    delete open[i];  
  }
  
  return res;
}
*/
void GenerateChildrenABS(TspState* state, myPriorityQueue* child, int beam) {
	NUM_NODES_EXPANDED++;
  for (int i = 1; i < nodes; i++) {
    if (state->IsChosen(i)) {
	  continue;
	}
    TspState *childstate = state->GetNewChild(i);
	childstate->UpdateFn(-1, state->_frm_mst_opt_node, state->GetFn());
	if (child->size() == beam && childstate->GetFn() >= child->bottom()->GetFn()) {
      delete childstate;
	  continue;
	}
    child->push (childstate);
    CUR_MEM++;
    if (CUR_MAX_MEM < CUR_MEM) {
      CUR_MAX_MEM = CUR_MEM;
	}
    if (child->size() > beam) {
	  TspState* s = child->popLast();
	  delete s;
	  CUR_MEM--;
	}
  }
}

int ABS(int BEAM, FILE* fp) {   
  time_t start_time = time(NULL);
  
  int result = MAX_RES;
  myPriorityQueue *q = new myPriorityQueue(1);
  //PriorityQueue *q = new PriorityQueue (myPriorityQueue::state_comp);
  TspState* s = CreateInitState();
  NUM_NODES_EXPANDED = 0;
  CUR_MAX_MEM = 1;
  CUR_MEM = 1;
  GenerateChildrenABS(s, q, BEAM);
  delete s;
  CUR_MEM--;
  if (BEAM > q->size()) {
	BEAM = q->size();
  }
  time_t end_time;
  int beam;
  for (beam = 1; beam <= BEAM; beam++) {
	TspState* s = q->top();
	q->pop();
	//printf("cur mem: %ld\n", CUR_MEM);
	result = BeamR(s, beam, fp, result, start_time);
	end_time = time(NULL);
    if (int(end_time - start_time) > MAX_T) {
      break;
	}
  }
  fprintf(fp, "%d %d %lf %ld %ld %d\n", beam < BEAM ? beam : BEAM, result, OPT * 100.0 / result, CUR_MAX_MEM, NUM_NODES_EXPANDED, int(end_time - start_time));
  fflush(fp);
  
  while (!q->empty ()) {
    TspState *s = (TspState *) q->top ();
    q->pop ();
    delete s;
  }
  delete q;
	  
  return result;
}
