/* 
 * Author: Sunandita Patra
 * email : sunandita.patra@cse.iitkgp.ernet.in
 *         sunandita.patra@gmail.com
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
 */
 
void GenerateChildren_modified_beam(KNState *state, myPriorityQueue *open, int beam, int res) {
  for(int i = 0; i < num_items; i++) {
    if(state->IsChildPossible (i)) {
   	  KNState *childstate = state->getChild(i);    	  
   	  if (childstate->GetFn() > res) {
   	    open->push (childstate);
	    if (open->size() > beam) {
		  KNState* s = open->popLast();
		  delete s;
	    }
      } else {
	    delete childstate;
      }
    }
  }
}

int modified_beam_h(time_t start_time, int k) {
  myPriorityQueue *open = new myPriorityQueue(0);
  
  int result = 0;
  open->push(CreateInitState());
  while(!open->empty()) {
    KNState* s = open->top();
    open->pop();

    if(s->IsGoalState()) {
      result = s->GetFn();
	  delete s;
      break;
	}
	
	GenerateChildren_modified_beam(s, open, k, result);
	delete s;/*
	if (time(NULL) - start_time > MAX_T) {
	  break;
	}*/
  }
  while (!open->empty()) {
    KNState* s = open->top();
    open->pop();
    delete s;
  }
  delete open;  
  return result;    
}

int modified_beam(FILE *fp, int k) {
  time_t start_time = time(NULL);
  int result = modified_beam_h(start_time, k);
  fprintf(fp, "%d %d %d\n", k, result, int(time(NULL) - start_time));
  fflush(fp);
  return result;
}
