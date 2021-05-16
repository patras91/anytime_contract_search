//Author: Sunandita Patra
//        sunandita.patra@gmail.com
#include <stdio.h>
#include <time.h>

void Normalize(double expR[], int NUM_QS) {
  double sum = 0;
  for(int i = 0; i<NUM_QS; i++) {
    sum += expR[i];
  }
  if(sum < 0.000001) {
    return;
  }
  for(int i = 0; i<NUM_QS; i++) {
    expR[i] = expR[i]/sum;
  }  
}

int min(int x, int y) {
  if(x>y) {
    return y;
  } else {
    return x;
  }
}
int adjustContract(int oldC, int newLimit, int k[], int ExpCount[], int NUM_QS, int SuspendFlag[], int refBeam, int levelNodeCount[]) {
  double expR[NUM_QS];
  double alpha = 1;
  int flag = 0;
  for(int i = 0; i<NUM_QS; i++) {
    expR[i] = k[i] + ExpCount[i]; 
  }
  Normalize(expR, NUM_QS);
  for(int i = 0; i<NUM_QS; i++) {
    SuspendFlag[i] = 0;
        //~ printf("\n k[%d] = %d expR[%d] = %lf",i,k[i],i,expR[i]);
    //~ if(flag) {
      //k[i] = refBeam;
      //continue;
    //~ }
    k[i] = k[i] + (int)(expR[i]*(newLimit - oldC));
    //~ k[i] = (int)(expR[i]*newLimit);
    //k[i] = min(k[i], refBeam);
    //~ if(k[i] == refBeam) {
      //flag = 1;
    //~ }
    //~ printf("\n k[%d] = %d %d",i,k[i], ExpCount[i]);
  }
  return newLimit;
}

double AnytimeContractSearch(const char *a, int MIN_C, int MAX_C, FILE *fp, int refBeam, int step) {
  int NUM_QS = nodes - 1 ;
  int k[NUM_QS], suspendFlag[NUM_QS], ExpCount[NUM_QS], levelNodeCount[NUM_QS];
  //~ myPriorityQueue* open[NUM_QS];
  Heap** open = new Heap*[NUM_QS];
  for (int i = 0; i < NUM_QS; i++) {
	k[i] = MIN_C/NUM_QS;
	open[i] = new Heap(0);
	suspendFlag[i] = 0;
    ExpCount[i] = 0;
    levelNodeCount[i] = 0;
    
  }
  //HHeap *clo = new HHeap(-1);
  HHashtable* openh = new HHashtable();
  //Heap *clo = new Heap(-1);
  HHashtable* clo = new HHashtable();
  time_t start_time = time(NULL);
  
  
  TspState* s = CreateInitState();
  open[0]->push (s);
  openh->push(s);
  levelNodeCount[0] = 1;
  int nodeCount = 1;
  
  //~ for(int i = 0; i < NUM_QS; i++) {
    //~ k[i] = MIN_C/NUM_QS;
    //~ printf("\n Initial K[%d] = %d ",i, k[i]);
    //~ open[i] = new myPriorityQueue(1);
    //~ suspendFlag[i] = 0;
    //~ ExpCount[i] = 0;
  //~ }
  //~ myPriorityQueue *ClosedList;
  //int bestSol = res;
  
  NUM_NODES_EXPANDED = 0;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;
  double bestSol = MAX_RES;
  //~ open[0]->push(CreateInitState());
  double oldSol = bestSol;
  bestSol = ContractSearch(MIN_C, k, bestSol, open, openh, clo, ExpCount, suspendFlag, &nodeCount, levelNodeCount, fp, start_time);
  //~ if(bestSol != oldSol) {

    //~ printf( "%d %d %lf %d %d\n", MIN_C, (int)bestSol, OPT*100/bestSol, (int)(time(NULL) - start_time), nodeCount);
  	//~ oldSol = bestSol;
 //~ }
  int oldC = MIN_C;
  
  for(int contract = MIN_C+step; bestSol>OPT && (int)(time(NULL) - start_time) < MAX_T; contract+=step) {
    oldC = adjustContract(oldC, contract, k, ExpCount, NUM_QS, suspendFlag, refBeam, levelNodeCount);
    bestSol = ContractSearch(contract, k, bestSol, open, openh, clo, ExpCount, suspendFlag, &nodeCount, levelNodeCount, fp, start_time);
       //~ printf( "%d %d %lf %d %d\n", contract, (int)bestSol, OPT*100/bestSol, (int)(time(NULL) - start_time), nodeCount);
    //~ printf("best sol = %lf\n",bestSol);
    //~ if(bestSol != oldSol) {
	    //~ char fname[200];
    //~ sprintf(fname, "%srank_profile_%d_%d_%d_%d.txt", a, MIN_C, MAX_C, step, contract);
    //~ //printf("%s\n",fname);
    //~ FILE *f = fopen(fname,"w");
    //~ for(int i = 0; i<NUM_QS; i++) {
      //~ fprintf(f, "%d %d \n", i, k[i]);
    //~ }
    //~ fclose(f);
      //~ fprintf(fp, "%d %d %lf %d %d\n", contract, (int)bestSol, OPT*100/bestSol, (int)(time(NULL) - start_time), nodeCount);
      //~ printf( "%d %d %lf %d %d\n", contract, (int)bestSol, OPT*100/bestSol, (int)(time(NULL) - start_time), nodeCount);
      //~ fflush(fp);
      //printf("a%da", res);
      //~ oldSol = bestSol;
    //~ }
  } 
  for(int i = 0; i < NUM_QS; i++) {
	open[i]->deleteAllStates();
    delete (open[i]);
  }
  delete open;
  openh->deleteAllEntries();
  delete openh;

  clo->deleteAllStates();
  delete clo;
  
  return bestSol;
}

