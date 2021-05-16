
/* 
 * Author: Sunandita Patra
 * email : sunandita.patra@cse.iitkgp.ernet.in
 *         sunandita.patra@gmail.com
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
 */

int scaled_modified_beam_h(time_t start_time, int k) {
  myPriorityQueue *open = new myPriorityQueue(0);
  
  int result = 0;
  open->push(CreateInitState());
  while(!open->empty()) {
    KNState* s = open->top();
    open->pop();

    if(s->IsGoalState()) {
      result = s->GetFn();
	  delete s;
	  k = 1;
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

int scaled_modified_beam(FILE *fp, int k) {
  time_t start_time = time(NULL);
  int result = scaled_modified_beam_h(start_time, k);
  fprintf(fp, "%d %d %d\n", k, result, int(time(NULL) - start_time));
  fflush(fp);
  return result;
}
