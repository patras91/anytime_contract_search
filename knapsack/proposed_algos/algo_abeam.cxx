/* 
 * Author: Sunandita Patra
 * email : sunandita.patra@cse.iitkgp.ernet.in
 *         sunandita.patra@gmail.com
 */
 
//keeps only best 'beam' number of nodes in the open list to be explored from the next level
void GenerateChildren_abeam(KNState *state, myPriorityQueue *open, myPriorityQueue *sus, int beam, int res) {
  for(int i = state->GetTopIndex() + 1; i < num_items; i++) {
    if(state->IsChildPossible (i)) {
   	  KNState *childstate = state->getChild(i);    	  
   	  if (childstate->GetFn() > res) {                 
   	    open->push (childstate);
      } else {
        delete childstate;
      }
    }
  }
}

short allEmpty(myPriorityQueue **open, int num) {
  int empty = 1;
  for(int i = 0; i<num+1; i++) {
    if(!open[i]->empty()) {
      empty = 0;
    }
  }
  return empty;
}

void moveSusToOpen(myPriorityQueue **open, myPriorityQueue **sus, int num, int bw, int result) {
  /*for(int i = level; i < num + 1; i++) {
    while (!open[level]->empty()) {
      KNState* s = open[i]->top();
      open[i]->pop();
      delete s;
    }
    while (!sus[level]->empty()) {
      KNState* s = sus[i]->top();
      sus[i]->pop();
      delete s;
    }
  }*/
  // adding nodes from suspend list to each level
  for(int i = 0; i < num + 1; i++) {
    while(open[i]->size() < bw && !sus[i]->empty()) {
	  KNState * s = sus[i]->top();
	  if(s->GetFn()>result) {
	    open[i]->push(s);
	    sus[i]->pop();
	  } else {
	    sus[i]->pop();
		delete s;
      }
    }
  }
}
//Anytime beam search
//uses suspend lists

int abeam(FILE *fp) {                                         //fp:output file pointer
  time_t start_time = time(NULL);                             //start time
  int result = 0;                                             //optimal result for knapsack:cost of the items in the sack
  int level, bw = 1;
  myPriorityQueue **open = new myPriorityQueue*[num_items+1];
  myPriorityQueue **sus = new myPriorityQueue*[num_items+1];         
  
  for (int i = 0; i < num_items+1; i++) {
    open[i] = new myPriorityQueue(0);
    sus[i] = new myPriorityQueue(0);
  }
  open[0]->push(CreateInitState());   
  level = 0;
  
  //~ while(!allEmpty(open, sus, num)) {
  while(!allEmpty(open, num_items)) {
	int exCount = 0;
    while(!((open[level])->empty()) && exCount < bw) {
      KNState* s = open[level]->top();
      open[level]->pop();
      exCount ++;
      if(s->IsGoalState()) {                          //fn > result check is required because in the same iteration of
                                                      //beam, after finding a solution we try to find solutions in the
                                                      //next level, which may have been added in open before the solution
                                                      //was found
        if(s->GetFn() > getLim()) {
          insertInList(s);
          fprintf(fp, "%d %d %ld\n", bw, getRes(), (time(NULL) - start_time));
          printf("%d %d %ld\n", bw, getRes(), (time(NULL) - start_time));
          fflush(fp);
        } else {
          //~ printf("invalid\n");
        }
        delete s;
        continue;         //this deletes the rest of the nodes in the current level's open list
      } else {
	    //~ printf("Not a goal state\n");
      }
	  if(s->GetFn()<=getLim()) {
		delete s;
	    continue;
	  }
	  GenerateChildren_abeam(s, open[level+1], sus[level+1], bw, getLim());
      delete s;
    }
    if(time(NULL) - start_time > MAX_T) {
	  break;
	}
	level++;
	if(level >= num_items) {
	  //printf("\n beam width = %d\n", bw);
	  bw++;   //increase beam width for finding the next solution
      //~ moveSusToOpen(open, sus, num, bw, result);
      level = 1; //start beam search from first level
	}
  }
  
  fprintf(fp, "%d %d %ld\n", bw, getRes(), (time(NULL) - start_time));
  printf("%d %d %ld\n", bw, getRes(), (time(NULL) - start_time));
  fflush(fp);
  
  
  //clearing the memory
  for(int i = 0; i<num_items + 1; i++) {
    while (!open[i]->empty()) {
      KNState* s = open[i]->top();
      open[i]->pop();
      delete s;
    }
    while (!sus[i]->empty()) {
      KNState* s = sus[i]->top();
      sus[i]->pop();
      delete s;
    }
  }
  delete open;  
  delete sus;
  return getRes();
}
