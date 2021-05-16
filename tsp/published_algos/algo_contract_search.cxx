//Author: Sunandita Patra
//        sunandita.patra@gmail.com
#include <stdio.h>

void GenerateChildrenContract(TspState *state, Heap *open[], HHashtable *qh, HHashtable *cl, double res) {
  NUM_NODES_EXPANDED++;
  Heap* q = open[state->GetDepth() + 1];
  for (int i = 1; i < nodes; i++) {
    if (state->IsChosen(i)) {
	  continue;
	}
    TspState *childstate = state->GetNewChild(i);   
    List* clo = cl->getList(childstate);
    int index;
    bool found = 0;
    if (clo && (index = clo->getIndex(childstate)) != -1) {   
	  found = 1;
      TspState* oldState = clo->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		clo->popIndex(index);
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  continue;
	}
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
      TspState* oldState = qht->getState(index);
	  if (oldState->GetGn() > childstate->GetGn()) {
		open[oldState->GetDepth()]->pop(oldState);		
		childstate->UpdateFn(oldState->GetHn(), oldState->_frm_mst_opt_node, state->GetFn());
		delete oldState;
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  qht->popIndex(index);
	  continue;
	}
		q->push(childstate);
		qht->updateIndex(index, childstate);
	  } else {
		delete childstate;
	  }
	} else {
		childstate->UpdateFn(-1, state->_frm_mst_opt_node, state->GetFn());
    if (childstate->GetFn() >= res) {
	  delete childstate;
	  continue;
	}
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

TspState *SelectBestAllLevels(Heap *open[], HHashtable *openh, int NUM_QS, int *level, int suspendflag[]) {
  TspState *best;
  int i = 0;
  while(i < NUM_QS && (open[i]->empty() || suspendflag[i] == 1)) {
      i++;
  } 
  if(i >= NUM_QS) {
    return NULL;
  }
  //~ printf("i=%d NUM_QS = %d\n",i, NUM_QS);
  best = open[i]->top();
  *level = i;
  int start = i;
  for(int i = start; i<NUM_QS; i++) {
    //~ if(suspendflag[i] == 1) {
      //~ continue;
    //~ } 
      //~ printf("i=%d\n",i);
    //~ TspState *s = open[i]->top();
	TspState *s;
    if(!open[i]->empty()) {
	  s = open[i]->top();
    } else {
	  //~ printf("continue\n");
      continue;
    }
    //~ printf("here1\n");
    if(best->GetFn() > s->GetFn()) {
      best = s;
      *level = i;
    } 
    //~ printf("here2\n");
  }
  open[*level]->pop();
  openh->pop(best);
  
  return best;
}
double ContractSearch(int contract, int k[], double res, Heap *open[], HHashtable *openh, HHashtable *clo, int expCount[], int suspendflag[], int *nodeCount, int levelNodeCount[], FILE *fp, time_t start_time) {
  //printf("\n IN contract search\n");
  double bestSol = res;
  int NUM_QS = nodes - 1;
  int level;
  bool oneExpanded = 1;
  //~ static int nodeCount = 0;
  while(oneExpanded) {
	  oneExpanded = 0;
      //~ int i = 0;
  /*    if(open[0]->empty()) {
      i++;
  } */
    //~ if(*nodeCount >= contract) {
	  //~ printf("%d %d\n",nodeCount, contract);
      //~ return bestSol; 
    //~ }
    TspState *s = SelectBestAllLevels(open, openh, NUM_QS, &level, suspendflag);
    
    if(s == NULL ) {
	  return bestSol;
	}
	
	if(s->GetFn() > bestSol) {
      delete s;
      CUR_MEM--;
      return bestSol;
    }
    //printf("s = %lf\n",s->GetFn());
    if(s->IsGoalState()) {
      bestSol = s->GetFn();
      fprintf(fp, "%d %d %lf %ld %ld %d\n", contract, (int)bestSol, OPT*100/bestSol, CUR_MAX_MEM, NUM_NODES_EXPANDED, (int)(time(NULL) - start_time));
      fflush(fp);
      delete s;
      CUR_MEM--;
      continue;
    }
    GenerateChildrenContract(s, open, openh, clo, bestSol);
	clo->push(s);
    *nodeCount = *nodeCount + 1;
    oneExpanded = 1;
    //~ levelNodeCount[level+1] += newNodeExpanded;
    expCount[level]  += 1;
    if(expCount[level] >= k[level]) {
       for(int i = 0; i<level;i++) {
         suspendflag[i] = 1;
       }
      suspendflag[level] = 1;
    }
  }
  return bestSol;
}
