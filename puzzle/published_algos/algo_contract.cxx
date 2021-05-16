//Author: Sunandita Patra
//        sunandita.patra@gmail.com
#include <stdio.h>

void GenerateChildrenContract(PuzState* state, HHeap* open[], HHashtable *qh, HHashtable *cl, int res) {
	NUM_NODES_EXPANDED++;
  HHeap* q = open[state->GetDepth() + 1];
  for (int i = 0; i < 4; i++) {
    if (!state->IsChildPossible(i)) {
	  continue;
	}
    PuzState *childstate = state->getChild(i, state->GetHn());   
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  continue;
	}
    List* clo = cl->getList(childstate);
    int index;
    bool found = 0;
    if (clo && (index = clo->getIndex(childstate)) != -1) {   
	  found = 1;
      PuzState* oldState = clo->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		clo->popIndex(index);
		delete oldState;
		q->push(childstate);
		qh->push(childstate);
	  } else {
		delete childstate;
	  }
	}
	if (found) {
	  continue;
	}
    List* qht = qh->getList(childstate);
    if (qht && (index = qht->getIndex(childstate)) != -1) {
      PuzState* oldState = qht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		open[oldState->GetDepth()]->pop(oldState);		
		delete oldState;
		q->push(childstate);
		qht->updateIndex(index, childstate);
	  } else {
		delete childstate;
	  }
	} else {
      q->push (childstate);
      if (qht) {
		qht->push(childstate);
	  } else {
        qh->push (childstate);
      }
      CUR_MEM++;
      if (CUR_MAX_MEM < CUR_MEM) {
		CUR_MAX_MEM = CUR_MEM;
	  }
    }
  }
}

PuzState *SelectBestAllLevels(HHeap *open[], HHashtable *qh, int NUM_QS, int *level, int suspendflag[]) {
  PuzState *best;
  int i = 0;
  while(i < NUM_QS && (open[i]->empty() || suspendflag[i] == 1)) {
      i++;
  } 
  if(i >= NUM_QS) {
    return NULL;
  }
  best = open[i]->top();
  *level = i;
  int start = i;
  for(int i = start; i<NUM_QS; i++) {
	PuzState *s;
	if(!open[i]->empty() && suspendflag[i] == 0) {
		s = open[i]->top();
    } else {
      continue;
    }
    if(best->GetFn() > s->GetFn()) {
      best = s;
      *level = i;
    } 
  }
  open[*level]->pop();
  qh->pop(best);
  return best;
}

int ContractSearch(int contract, int k[], int res, HHeap *q[], HHashtable *qh, HHashtable *clo, int expCount[], int suspendflag[], FILE *fp, time_t start_time) {
  //long nexps = 0;
  time_t end_time;
  //~ int iter = I;
  bool oneExpanded = 1;
  int level;
  while(oneExpanded) {
	//printf("\n in while contract = %d", contract);
    oneExpanded = 0;
    //bool goalFound = 0;
    
    PuzState *s = SelectBestAllLevels(q,qh,res,&level, suspendflag);
    
    if(s == NULL ) {
	  return res;
	}
	
	if(s->GetFn() >= res) {
      delete s;
      CUR_MEM--;
      return res;
    }
    
	if (s->IsGoalState ()) {
        res = s->GetFn ();
        //~ fprintf(fp, "%d %d %lf %ld %d\n", contract, res, (OPT * 100.0) / res, nexps, int(end_time - start_time)); 
        fprintf(fp, "%d %d %lf %ld %ld %d\n", contract, res, OPT*100.0/res, CUR_MAX_MEM, NUM_NODES_EXPANDED, (int)(time(NULL) - start_time));
        //printf( "%d %d %lf %ld %ld %d\n", contract, res, OPT*100.0/res, CUR_MAX_MEM, NUM_NODES_EXPANDED, (int)(time(NULL) - start_time));
        fflush(fp);
        delete s;
        CUR_MEM--;
        //goalFound = 1;
	    continue;
	}
	
	  ACGenerateChildrenGraph(s, q, clo, res, qh);
	  clo->push(s);
	  //nexps++;
	  expCount[level]++;
	  oneExpanded = 1;
	  
	 if(expCount[level] >= k[level]) {
      suspendflag[level] = 1;
	}
	/*for (int i = 0; i < res; i++) {
    for (int j = expCount[i]; j < k[i]; j++) {
	  if (q[i]->empty()) {
		break;
	  }
	  PuzState *s = q[i]->top ();
	  q[i]->pop ();
	  qh->pop(s);
	  if (s->GetFn () >= res) {
		delete s;
		CUR_MEM--;
	    break;
	  }
      end_time = time(NULL);
	  if (s->IsGoalState ()) {
        res = s->GetFn ();
        //~ fprintf(fp, "%d %d %lf %ld %d\n", contract, res, (OPT * 100.0) / res, nexps, int(end_time - start_time)); 
        fprintf(fp, "%d %d %lf %ld %ld %d\n", contract, res, OPT*100.0/res, CUR_MAX_MEM, NUM_NODES_EXPANDED, (int)(time(NULL) - start_time));
        //printf( "%d %d %lf %ld %ld %d\n", contract, res, OPT*100.0/res, CUR_MAX_MEM, NUM_NODES_EXPANDED, (int)(time(NULL) - start_time));
        fflush(fp);
        delete s;
        CUR_MEM--;
        goalFound = 1;
	    break;
	  }
      if (int(end_time - start_time) > MAX_T) {
	    break;
	  }
	  ACGenerateChildrenGraph(s, q, clo, res, qh);
	  clo->push(s);
	  nexps++;
	  expCount[i]++;
	  oneExpanded = 1;
    }
      if (goalFound || int(end_time - start_time) > MAX_T) {
	    break;
	  }
	}*/
    if (int(end_time - start_time) > MAX_T) {
	  break;
	}
  }
  
  return res;
}
/*
int ContractSearch(int contract, int k[], int res, Heap *open[], myhashtable *qh, myhashtable *clo, int expCount[], int suspendflag[], FILE *fp, time_t start_time) {
  int bestSol = res;
  int level;
  static int nodeCount = 0;

  
  while(1) {
      int i = 0;
      if(open[0]->empty()) {
      i++;
  } 
    if(nodeCount >= contract) {
      return bestSol; 
    }
    PuzState *s = SelectBestAllLevels(open, qh, MAX_D, &level, suspendflag);
    
    if(s == NULL || s->GetFn() > bestSol) {
      return bestSol;
    }
    //~ printf("fn = %d\n", s->GetFn());
    if(s->IsGoalState()) {
      bestSol = s->GetFn();
      continue;
    }
    GenerateChildrenContract(s, open, qh, clo, bestSol);
    nodeCount++;
    expCount[level]++;
    if(expCount[level] >= k[level]) {
      suspendflag[level] = 1;
    }
  }
  return bestSol;
} */
