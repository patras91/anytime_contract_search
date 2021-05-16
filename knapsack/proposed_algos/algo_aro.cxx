/* 
 * Author: Sunandita Patra
 * email : sunandita.patra@cse.iitkgp.ernet.in
 *         sunandita.patra@gmail.com
 */
 
 //
void GenerateChildren_amb(KNState *state, myPriorityQueue *open, myPriorityQueue *sus, int beam, int res) {
  for(int i = state->GetTopIndex()+1; i < num_items; i++) {
    if(state->IsChildPossible (i)) {
   	  KNState *childstate = state->getChild(i);    	  
   	  if (childstate->GetFn() > res) {
   	    open->push (childstate);
        if (open->size() > beam) {
          KNState* s = open->popLast();
          sus->push(s);
        }
      } else {
        delete childstate;
      }
    }
  }
}

int amb(FILE *fp,  int k) {
  time_t start_time = time(NULL);
  myPriorityQueue *open = new myPriorityQueue(0);
  myPriorityQueue *sus = new myPriorityQueue(0);
  
  k = 1;
  int result = 0;
  open->push(CreateInitState());
  while(!open->empty() || !sus->empty()) {
	if(open->empty()) {
	  k++;
	  printf("k = %d\n",k);
	  while(open->size() < k  && !sus->empty()) {
	    KNState *s = sus->top();
		open->push(s);
		sus->pop();
      }  
	}
    KNState* s = open->top();
    open->pop();
    if(result >= s->GetFn()) {
      delete s;
	  continue;
	}
    if(s->IsGoalState()) {
	  //~ s->print();
      result = s->GetFn();
	  delete s;
	  fprintf(fp, "%d %d %ld\n", k, result, (time(NULL) - start_time));
	  printf("%d %d %ld\n", k, result, (time(NULL) - start_time));
	  fflush(fp);/*
      if(sus->empty()) {
	    break;
	  } else {
		fprintf(fp, "%d %d %d\n", k, result, int(time(NULL) - start_time));
		printf("%d %d %d\n", k, result, int(time(NULL) - start_time));
		fflush(fp);
	    while(!open->empty()) {
		  KNState *s = open->top();
		  open->pop();
		  delete s;
		}
		continue;
	  }*/
	  continue;
	}
	
	GenerateChildren_amb(s, open, sus, k, result);
	delete s;
	if (time(NULL) - start_time > MAX_T) {
	  break;
	}
  }
  while (!open->empty()) {
    KNState* s = open->top();
    open->pop();
    delete s;
  }
  delete open;  
  while (!sus->empty()) {
    KNState* s = sus->top();
    sus->pop();
    delete s;
  }
  delete sus;  
  fprintf(fp, "%d %d %ld\n", k, result, (time(NULL) - start_time));
  fflush(fp);
  return result;    
}

