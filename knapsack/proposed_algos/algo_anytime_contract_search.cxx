//Author: Sunandita Patra
//        sunandita.patra@gmail.com
#include <stdio.h>
#include <time.h>

void Normalize(double expR[], int NUM_QS) {
  double sum = 0;
  for(int i = 0; i<NUM_QS; i++) {
        //~ printf("\n %lf", expR[i]);
    sum += expR[i];
    //~ printf("\n sum = %lf", sum);
  }
  //~ printf("\n sum = %lf", sum);
  if(sum < 0.000001) {
    return;
  }
  for(int i = 0; i<NUM_QS; i++) {
    expR[i] = expR[i]/sum;
    //~ printf("\n %lf", expR[i]);
  }  
  //~ printf("\n Normalized");
}

int min(int x, int y) {
  if(x>y) {
    return y;
  } else {
    return x;
  }
}
int adjustContract(int oldC, int newLimit, int k[], int ExpCount[], int NUM_QS, int SuspendFlag[], int refBeam) {
  double expR[NUM_QS];
  double alpha = 0.5;
  int flag = 0;
  for(int i = 0; i<NUM_QS; i++) {
    expR[i] = alpha*k[i] + (1-alpha)*ExpCount[i];
  }
  Normalize(expR, NUM_QS);
  for(int i = 0; i<NUM_QS; i++) {
    SuspendFlag[i] = 0;
         //printf("\n k[%d] = %d expR[%d] = %lf",i,k[i],i,expR[i]);
    if(flag) {
      k[i] = refBeam;
      continue;
    }
    //k[i] = k[i] + (int)(expR[i]*(newLimit - oldC));
    //k[i] = min(k[i], refBeam);
    k[i] = expR[i]*newLimit;
    if(k[i] == refBeam) {
      flag = 1;
    }
     //printf("\n k[%d] = %d %d",i,k[i], ExpCount[i]);
  }
  return newLimit;
}

double AnytimeContractSearch(int MIN_C, int MAX_C, FILE *fp, int refBeam, int step) {
  int NUM_QS = num_items+1;
  int k[NUM_QS], suspendFlag[NUM_QS], ExpCount[NUM_QS];
  myPriorityQueue* open[NUM_QS];

  time_t start_time = time(NULL);
  for(int i = 0; i < NUM_QS; i++) {
    k[i] = MIN_C/NUM_QS;
    //~ printf("\n Initial K[%d] = %d ",i, k[i]);
    open[i] = new myPriorityQueue(0);
    suspendFlag[i] = 0;
    ExpCount[i] = 0;
  }
  //~ myPriorityQueue *ClosedList;
  //int bestSol = res;
  double bestSol = 0;
  double oldSol = 0;
  open[0]->push(CreateInitState());

  bestSol = ContractSearch(MIN_C, k, bestSol, open, ExpCount, suspendFlag);
  if(bestSol != oldSol ) {
  	fprintf(fp, "%d %lf %d\n", MIN_C, bestSol, (int)(time(NULL) - start_time));
  	oldSol = bestSol;
  }
  int oldC = MIN_C;
  
  for(int contract = MIN_C+step; contract < MAX_C; contract+=step) {
    oldC = adjustContract(oldC, contract, k, ExpCount, NUM_QS, suspendFlag, refBeam);
    bestSol = ContractSearch(contract, k, bestSol, open, ExpCount, suspendFlag);
    if(oldSol!=bestSol) {
        char fname[100];
    	sprintf(fname, "testcases/random/rankProfile/rank_profile_%d_%d_%d_%d_%d.txt", num_items, MIN_C, MAX_C, step, contract);
    	FILE *f = fopen(fname,"w");
    	for(int i = 0; i<NUM_QS; i++) {
       	fprintf(f, "%d %d \n", i, k[i]);
    	}
    	fclose(f);
    	fprintf(fp, "%d %lf %d\n", contract, bestSol, (int)(time(NULL) - start_time));
    	printf("%d %lf %d\n", contract, bestSol, (int)(time(NULL) - start_time));
    	oldSol = bestSol;
    }
  } 
  return bestSol;
}


