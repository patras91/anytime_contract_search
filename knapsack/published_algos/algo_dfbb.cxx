/*
 * Author: 
 * Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
 * Sunandita Patra (sunandita DOT patra AT cse DOT iitkgp DOT ernet DOT in; sunandita DOT patra AT gmail DOT com) 
*/

void GenerateChildrenDFBB (KNState * state, myPriorityQueue * q, int res) {
  for (int i = state->GetTopIndex() + 1; i < num_items; i++) {
    if(state->IsChildPossible (i)) {
   	  KNState *childstate = state->getChild(i);    	  
      if (childstate->GetFn() > res) {
        //~ childstate->print();
  	    q->push (childstate);
      } else {
        delete childstate;
      }
    }
  }
}

int DFBB (KNState* start, FILE* fp) {
  time_t start_time;
  time_t end_time;
  start_time = time(NULL);
  
  int res = 0;
  myPriorityQueue *q = new myPriorityQueue(2);
  q->push(start);
  
  while (!q->empty ()) {
    KNState *n = (KNState *) q->top ();
    q->pop ();
    if (n->GetFn () <= getLim()) {
      delete n;
      continue;
    }
    if (n->IsGoalState ()) {
      insertInList(n);
      //~ n->print();
      //q->pruneNodes(res);
      end_time = time(NULL);
      delete n;
      fprintf(fp, " %d %d\n", getRes(), int(end_time - start_time));
      printf( " %d %d\n", getRes(), int(end_time - start_time));
      fflush(fp);
      continue;
    }
    end_time = time(NULL);
    if (end_time - start_time > MAX_T) {
      delete n;
      fprintf(fp, " %d %d\n", getRes(), int(end_time - start_time));
      printf( " %d %d\n", getRes(), int(end_time - start_time));
      fflush(fp);
      break;
    }
    GenerateChildrenDFBB (n, q, getLim());
    delete n;
  }
  while (!q->empty ()) {
    KNState *n = (KNState *) q->top ();
    q->pop ();
    delete n;
  }
  delete q;
  printf("\n time = %d", int(time(NULL) - start_time));
  return getRes();
}
