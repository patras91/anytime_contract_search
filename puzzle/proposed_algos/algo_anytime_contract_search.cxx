//Author: Sunandita Patra
//        sunandita.patra@gmail.com
#include <stdio.h>
#include <time.h>

void Normalize(double expR[], int bestSol) {
  double sum = 0;
  for(int i = 0; i<bestSol; i++) {
        //~ printf("\n %lf", expR[i]);
    sum += expR[i];
    //~ printf("\n sum = %lf", sum);
  }
  //~ printf("\n sum = %lf", sum);
  if(sum < 0.000001) {
    return;
  }
  for(int i = 0; i<bestSol; i++) {
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

int adjustContract(int oldC, int newLimit, int k[], int ExpCount[], int SuspendFlag[], int bestSol) {
  double expR[bestSol];
  
  switch (mode) {
   case 4:
   case 1: for(int i = 0; i<bestSol;i++) {
			 expR[i] = (k[i] + ExpCount[i]);
		   }
		   Normalize(expR, bestSol);
		   for(int i = 0; i<bestSol;i++) {
		      k[i] = k[i] + expR[i]*(newLimit - oldC);
		   }
		   break;
   case 2: for(int i = 0; i<bestSol; i++) {
				expR[i] = (k[i] + ExpCount[i])/(bestSol-i);
		   }
		   Normalize(expR, bestSol);
		   for(int i = 0; i<bestSol;i++) {
		      k[i] = k[i] + expR[i]*(newLimit - oldC);
		   }
  		   break;
   case 3: for(int i = 0; i<bestSol;i++) {
		     k[i]=k[i]+(newLimit-oldC)/bestSol;
		   }
		   break;
  }
  
  for(int i = 0; i<bestSol; i++) {
    SuspendFlag[i] = 0;
  }
  return newLimit;
}

/*int adjustContract(int oldC, int newLimit, int k[], int ExpCount[], int NUM_QS, int SuspendFlag[], int bestSol) {
  double expR[NUM_QS];
  double alpha = 0.5;
  int flag = 0;
  for(int i = 0; i<NUM_QS; i++) {
    expR[i] = (k[i] + ExpCount[i])/(MAX_D-i);
  }
  Normalize(expR, NUM_QS, bestSol);
  for(int i = 0; i<NUM_QS; i++) {
    SuspendFlag[i] = 0;
    //~ printf("\n k[%d] = %d expR[%d] = %lf",i,k[i],i,expR[i]);
    /*if(flag) {
      k[i] = refBeam;
      continue;
    }*/
    //~ k[i] = k[i] + expR[i]*(newLimit - oldC);
    //k[i]=k[i]+(newLimit-oldC)/NUM_QS;
    //k[i] = expR[i]*newLimit;
    //k[i] = min(k[i], refBeam);
    /*if(k[i] == refBeam) {
      flag = 1;
    }*/
    //~ printf("\n k[%d] = %d %d",i,k[i], ExpCount[i]);
  //~ }
  //~ return newLimit;
//~ }

int anytimeContractSearch( int res, FILE *fp, int spec[], int ref, int count) {
  //~ int NUM_QS = res;
  printf("OPT = %d\n",OPT);
  int k[MAX_D], suspendFlag[MAX_D], ExpCount[MAX_D];
  //~ myPriorityQueue* open[MAX_D];

  time_t start_time = time(NULL);
  HHeap** open = new HHeap*[MAX_D];
  
  for (int i = 0; i < MAX_D; i++) {
	k[i] = spec[0]/MAX_D;
	open[i] = new HHeap(0);
	suspendFlag[i] = 0;
    ExpCount[i] = 0;
  }
  //~ IntervalHeap* open = new IntervalHeap(0);
  //~ IntervalHeap* child = new IntervalHeap(0);
  HHashtable* qh = new HHashtable();
  //~ myhashtable* ch = new myhashtable();
  HHashtable* clo = new HHashtable();  
  
  PuzState* s = CreateInitState();
  open[0]->push (s);
  qh->push(s);
  res = MAX_D;
  NUM_NODES_EXPANDED = 1;
  CUR_MEM = 1;
  CUR_MAX_MEM = 1;

//~ 
  //~ for(int i = 0; i < NUM_QS; i++) {
    //~ k[i] = MIN_C/NUM_QS;
    //~ //open[i] = new myPriorityQueue(1);
    //~ suspendFlag[i] = 0;
    //~ ExpCount[i] = 0;
  //~ }
  int bestSol = res;
  //~ int oldSol = bestSol;

  bestSol = ContractSearch(spec[0], k, bestSol, open, qh, clo, ExpCount, suspendFlag, fp, start_time);
  //~ printf("best sol = %d\n", bestSol);
  //~ if(oldSol != bestSol) {
  	//fprintf(fp, "%d %d %lf %ld %ld %d\n", MIN_C, bestSol, OPT*100.0/bestSol, CUR_MAX_MEM, NUM_NODES_GENERATED, (int)(time(NULL) - start_time));
  	//~ oldSol = bestSol;
  //~ }
  int oldC = spec[0];
  
  int contract = spec[1];
  
  for(int i = 1; bestSol >= OPT && (int) (time(NULL) - start_time) < MAX_T && contract > 0 && i < count; i++) {
    contract = spec[i];
    int contract_tmp = oldC;
	while(contract - contract_tmp > ref) {
	  contract_tmp = contract_tmp + ref;
	  oldC = adjustContract(oldC, contract_tmp, k, ExpCount, suspendFlag, bestSol);
      bestSol = ContractSearch(contract_tmp, k, bestSol, open, qh, clo, ExpCount, suspendFlag, fp, start_time);
	}
    oldC = adjustContract(oldC, contract, k, ExpCount, suspendFlag, bestSol);
    bestSol = ContractSearch(contract, k, bestSol, open, qh, clo, ExpCount, suspendFlag, fp, start_time);
    //~ printf("contract = %d sol = %d time  = %d\n", contract, bestSol, (int)(time(NULL) - start_time));
    //~ if(oldSol != bestSol) {
        //~ char fname[100];
    	//~ sprintf(fname, "rankProfile/rank_profile_%d_%d_%d_%d_%d.txt", problem, MIN_C, MAX_C, step, contract);
    	//~ FILE *f = fopen(fname,"w");
    	//~ for(int i = 0; i<NUM_QS; i++) {
      		//~ fprintf(f, "%d %d \n", i, k[i]);
    	//~ }
    	//~ fclose(f);
		//~ fprintf(fp, "%d %d %lf %ld %ld %d\n", contract, bestSol, OPT*100.0/bestSol, CUR_MAX_MEM, NUM_NODES_GENERATED, (int)(time(NULL) - start_time));

    	//fflush(fp);
    	//~ oldSol = bestSol;
    //~ }
  } 
  for (int i = 0; i < MAX_D; i++) {
    open[i]->deleteAllStates();
    delete open[i];
  }
  delete open;
  qh->deleteAllEntries();
  delete qh;

  clo->deleteAllStates();
  delete clo;
  
  return res;
}
