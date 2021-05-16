//Author: Sunandita Patra
//        sunandita.patra@gmail.com
#include <stdio.h>

void GenerateChildrenContract(KNState *state, myPriorityQueue *open, int res) {
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

KNState *SelectBestAllLevels(myPriorityQueue *open[], int NUM_QS, int *level, int suspendflag[]) {
  //~ printf("selecting...\n");
  KNState *best;
  int i = 0;
  while(i < NUM_QS && (open[i]->empty() || suspendflag[i] == 1)) {
      //~ printf("sfssfwfwefwefewfe\n");
      i++;
  } 
  if(i >= NUM_QS) {
    return NULL;
  }
  best = open[i]->top();
  *level = i;
  int start = i;
  for(int i = start; i<NUM_QS; i++) {
    //~ if(suspendflag[i] == 1) {
      //~ continue;
    //~ } 
    KNState *s = open[i]->top();
    if(!s) {
      continue;
    }
    if(best->GetFn() < s->GetFn()) {
      best = s;
      *level = i;
    } 
  }
  open[*level]->pop();
  //~ printf("Selected.\n");
  return best;
}
double ContractSearch(int contract, int k[], double res, myPriorityQueue *open[], int expCount[], int suspendflag[]) {
  double bestSol = res;
  int NUM_QS = num_items+1;
  int level;
  static int nodeCount = 0;
  while(1) {
    //~ printf("\nwhile");
      int i = 0;
      if(open[0]->empty()) {
      //~ printf("aaaaaaaaaaaaaaaaaaaaaaaa\n");
      i++;
  } 
    if(nodeCount >= contract) {
      //~ printf("\n nodecoount = %d contract = %d", nodeCount, contract);
      return bestSol; 
    }
    KNState *s = SelectBestAllLevels(open, NUM_QS, &level, suspendflag);
    
    if(s == NULL || s->GetFn() < bestSol) {
      //~ printf("here");
      return bestSol;
    }
    //~ printf("fn = %lf\n",s->GetFn());
    if(s->IsGoalState()) {
      bestSol = s->GetFn();
      //~ expCount[level]++;
      continue;
    }
    //~ printf("trying to generate children\n");
    GenerateChildrenContract(s, open[level + 1], bestSol);
    nodeCount++;
    expCount[level]++;
    if(expCount[level] >= k[level]) {
      //~ printf("\n erwerswrs");
      //~ for(int i = 0; i<level;i++) {
        //~ suspendflag[i] = 1;
      //~ }
      suspendflag[level] = 1;
    }
  }
  //~ printf("\n returning");
  return bestSol;
}

