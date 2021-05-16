/* 
 * Author: Sunandita Patra
 * email : sunandita.patra@cse.iitkgp.ernet.in
 *         sunandita.patra@gmail.com
 */

//keeps only best 'beam' number of nodes in the open list to be explored from the next level
void GenerateChildren_beam(KNState *state, myPriorityQueue *open, int beam, int res) {
  for(int i = state->GetTopIndex() + 1; i < num_items; i++) {
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

//knapsack problem using beam search
int beam(FILE *fp, int bw) {                                  //fp:output file pointer
                                                              //bw:beam width
  time_t start_time = time(NULL);                             //start time
  int result = 0;                                             //optimal result for knapsack:cost of the items in the sack
  int solved;
  myPriorityQueue *open_cl = new myPriorityQueue(0);          //list of nodes in the current level that are to be explored
  myPriorityQueue *open_nl = new myPriorityQueue(0);          //list of nodes to be explored in the level that are to be explored
  open_cl->push(CreateInitState());   
  printf("\n in beam");
  while(1) {
    while(!open_cl->empty()) {
      KNState* s = open_cl->top();
      open_cl->pop();

      if(s->IsGoalState()) {
        result = s->GetFn();
        delete s;
        solved = 1;
        break;
	  }
	  GenerateChildren_beam(s, open_nl, bw, result);
    }
    if(solved) {
      break;
    }
    myPriorityQueue *t = open_cl;
    open_cl = open_nl;
    open_nl = t;
    //~ printf("\n here----------------------");
    //~ open_cl->print();
  }
  
  //clearing the memory
  while (!open_cl->empty()) {
    KNState* s = open_cl->top();
    open_cl->pop();
    delete s;
  }
  while (!open_nl->empty()) {
    KNState* s = open_nl->top();
    open_nl->pop();
    delete s;
  }
  //~ delete open_cl;  
  //~ delete open_nl;
    
  fprintf(fp, "%d %d %d\n", bw, result, int(time(NULL) - start_time));
  fflush(fp);
  return result;
}
