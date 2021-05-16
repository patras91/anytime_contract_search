/*
 * Author: Sunandita Patra
 * email: sunandita.patra@cse.iitkgp.ernet.in
*/

//giving segmentation faults
double BeamR(int beam, int result, FILE* fp, time_t start_time) {
  myPriorityQueue **open = new myPriorityQueue*[num_items - 1];
  for (int i = 0; i < num_items - 1 ; i++) {
    open[i] = new myPriorityQueue(1);
  }
  time_t end_time;
  open[0]->push(CreateInitState());
  for (int level = 0; level < num_items - 2; level++) {
	myPriorityQueue* t = open[level];
    while(t->next != open[level]) {
	  KNState* s = t->next->s;
	  GenerateChildren_modified_beam(s, open[level + 1], beam, result);
	  t = t->next;
	  end_time = time(NULL);
	  if (end_time - start_time > MAX_T) {
		break;
	  }
    }
	if (end_time - start_time > MAX_T) {
	  break;
	}
  }
  KNState* s = open[num_items - 2]->top();
  if (s) {
    result = s->GetFn();
    time_t end_time = time(NULL);
    fprintf(fp, "%d %d %d\n", beam, result, int(end_time - start_time));
    fflush(fp);
  }
  
  for (int i = 0; i < num_items - 1; i++) {
    while (!open[i]->empty()) {
      KNState* s = open[i]->top();
      open[i]->pop();
      delete s;
    }
    delete open[i];
  }
  delete open;
  
  return result;
}

//iterative beam search for knapsack
int itBeam(int BEAM, FILE* fp) {   
  time_t start_time = time(NULL);
   
  int result = 0;
  int beam;
  for (beam = 1; beam <= BEAM; beam++) {
	result = BeamR(beam, result, fp, start_time);
    if (int(time(NULL) - start_time) > MAX_T) {
      break;
	}  
  }
  fprintf(fp, "%d %d %d\n", beam - 1, result, int(time(NULL) - start_time));
  fflush(fp);
	  
  return result;
}

