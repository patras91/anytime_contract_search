// Code for puz problem.
// Author: satya@cse.iitkgp.ernet.in

#include <stdio.h>
#include <time.h>
//#include "puz_state.h"
#include <new>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
//#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <sys/wait.h>
#include <string.h>
using namespace std;
#include <string>
#include <map>


int _size;
int MAX_D;
int result_ana;
int mode;
//#include "puz_state.h"
#include "puz_state.cxx"

//#include "library/my_priority_queue.h"
#include "library/my_priority_queue.cxx"
bool Compare(PuzState* a, PuzState* b, short flag) {
  switch(flag) {
	case 0:
	  return myPriorityQueue::state_comp(a, b);
	case 1:
	  return a->GetANAkey() < b->GetANAkey();
	case 2:  // dfbb
	  if (a->GetDepth() < b->GetDepth()) {
		return 1;
	  }
	  if (a->GetDepth() > b->GetDepth()) {
		return 0;
	  }
	  return a->GetFn() > b->GetFn();
	default:
	  return 0;
  }
}
#include "library/my_heap.cxx"
#include "library/my_heirarchical_heap.cxx"
//#include "library/my_interval_heap.cxx"

#include "library/my_list.cxx"
#include "library/my_hashtable.cxx"
#include "library/HHashtable.cxx"

#define MAX_T 3840
int OPT;

#include "read_data.cxx"

PuzState* CreateInitState() {
  PuzState *state = new PuzState(1);
  state->Initialize(_puzzle);
  return state;
}

//---------------------------------------------------------------------------------
long NUM_NODES_EXPANDED;
long CUR_MEM;
long CUR_MAX_MEM;

#include "published_algos/algo_a.cxx"
//#include "published_algos/algo_ara.cxx"
#include "published_algos/algo_awa_family.cxx"
#include "published_algos/algo_acs_family.cxx"
#include "published_algos/algo_ana.cxx"
//#include "published_algos/algo_mag.cxx"
//#include "published_algos/algo_mawa.cxx"
#include "published_algos/algo_beam_stack.cxx"
#include "published_algos/algo_dfbb.cxx"
#include "published_algos/algo_beam_family.cxx"
#include "published_algos/algo_contract.cxx"
//#include "published_algos/algo_beam_family1.cxx"

#include "proposed_algos/algo_anytime_contract_search.cxx"
#include "proposed_algos/algo_ibs_family.cxx"

#include "proposed_algos/algo_aps_family.cxx"
//#include "proposed_algos/algo_ibsa.cxx"
//#include "proposed_algos/algo_ibsn.cxx"
//#include "proposed_algos/algo_ibsc.cxx"
//#include "proposed_algos/algo_itbc.cxx"

//#include "proposed_algos/algo_ibn.cxx"
//#include "proposed_algos/algo_ib.cxx"

//#include "proposed_algos/new/algo_ab.cxx"
//#include "proposed_algos/new/algo_pawa.cxx"
//-------------------------------------------------------------------------------

bool printName;

int runAStar(const char *a) {
  char fname[100];
  sprintf(fname, "%s_a.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = AStar(fp);
  fclose(fp);
  return res;
}

int runAWA(const char *a) {
  char fname[100];
  sprintf(fname, "%s_awa.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = AWA(fp);
  fclose(fp);
  return res;
}
/*
void runAWAT(const char *a) {
  char fname[100];
  sprintf(fname, "%s_awa.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  
  time_t start_time = time(NULL);
  pid_t pid = fork();
  if (pid == 0) {
    //printf("child process begun\n", pid, status);
    AWA(fp);
    exit(0);
  } else if (pid > 0) {
    int status;
	if (waitpid (pid, &status, 0) != pid) {
      status = -1;
    }
    if (status == -1) {
      fprintf(fp, "wrong child %d\n", int(time(NULL) - start_time));
    } else if (status == 0) {
      fprintf(fp, "0 0 0 %d\n", int(time(NULL) - start_time));
	} else {
      fprintf(fp, "0 1 0 %d\n", int(time(NULL) - start_time));
	}
  } else {
    printf("%d : fork failed\n", pid);
  }
  fclose(fp);
}
*/
int runAWAS(const char *a) {
  char fname[100];
  sprintf(fname, "%s_awas.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = AWAS(fp);
  fclose(fp);
  return res;
}

int runANA(const char *a) {
  char fname[100];
  sprintf(fname, "%s_ana.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = ANA(fp);
  fclose(fp);
  return res;
}
/*
void runANAT(const char *a) {
  char fname[100];
  sprintf(fname, "%s_ana.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  
  time_t start_time = time(NULL);
  pid_t pid = fork();
  if (pid == 0) {
    //printf("child process begun\n", pid, status);
    ANA(fp);
    exit(0);
  } else if (pid > 0) {
    int status;
	if (waitpid (pid, &status, 0) != pid) {
      status = -1;
    }
    if (status == -1) {
      fprintf(fp, "wrong child %d\n", int(time(NULL) - start_time));
    } else if (status == 0) {
      fprintf(fp, "0 0 %d\n", int(time(NULL) - start_time));
	} else {
      fprintf(fp, "0 1 %d\n", int(time(NULL) - start_time));
	}
  } else {
    printf("%d : fork failed\n", pid);
  }
  fclose(fp);
}
*/
int runACP(const char *a) {
  char fname[100];
  sprintf(fname, "%s_acp.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = ACP(fp);
  fclose(fp);
  return res;
}

int runACS(const char *a) {
  char fname[100];
  sprintf(fname, "%s_acs.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = ACS(fp);
  fclose(fp);
  return res;
}

int runACA(const char *a) {
  char fname[100];
  sprintf(fname, "%s_aca.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = ACA(fp);
  fclose(fp);
  return res;
}

int runACI(const char *a, int I) {
  char fname[100];
  sprintf(fname, "%s_aci_%d.out", a, I);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = ACI(I, fp);
  fclose(fp);
  return res;
}

/*
int runPAWA(int p, const char *a) {
  char fname[100];
  sprintf(fname, "%s_pawa_%d.out", a, p);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = PAWA(p, fp);
  fclose(fp);
  return res;
}

int runAB(const char *a) {
  char fname[100];
  sprintf(fname, "%s_ab.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = AnytimeBeam(fp);
  fclose(fp);
  return res;
}

int runARA(double epsilon, double delta, const char *a) {
  char fname[100];
  sprintf(fname, "%s_ara.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = ARA(epsilon, delta, fp);  // 1.08, 0.02
  fclose(fp);
  return res;
}
*/
int runBeam(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_beam.out", a);
  if (printName) {
    printf("%s %d\n", fname, beam);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = Beam(beam, fp);
  fclose(fp);
  return res;
}
/*
int runBeam1(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_beam1.out", a);
  if (printName) {
    printf("%s %d\n", fname, beam);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = Beam1(beam, fp);
  fclose(fp);
  return res;
}
*/
int runBS(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_bs_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = Beam_Stack(beam, fp);
  fclose(fp);
  return res;
}

int runDFBB(const char *a) {
  char fname[100];
  sprintf(fname, "%s_dfbb.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = DFBB(fp);
  fclose(fp);
  return res;
}
/*
int runMAWA(const char *a, int MAX) {
  char fname[100];
  sprintf(fname, "%s_mawa_%d.out", a, MAX);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = MAWA(MAX, fp);
  fclose(fp);
  return res;
}
*/
int runItB(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_itb_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = ItB(beam, step, fp);
  fclose(fp);
  return res;
}
/*
int runItB1(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_itb1_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = ItB1(beam, step, fp);
  fclose(fp);
  return res;
}
*/
int runIBS(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_ibs_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = IBS(beam, step, fp);
  fclose(fp);
  return res;
}

int runIBSI(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_ibsi_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = IBSI(beam, step, fp);
  fclose(fp);
  return res;
}

int runIBSIM(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_ibsim_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = IBSIM(beam, step, fp);
  fclose(fp);
  return res;
}

int runIBSF(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_ibsf_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = IBSF(beam, step, fp);
  fclose(fp);
  return res;
}
/*
int runIBSA(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_ibsa_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = IBSA(beam, step, fp);
  fclose(fp);
  return res;
}

int runIBSN(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_ibsn.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = IBSN(beam, fp);
  fclose(fp);
  return res;
}

int runIBN(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_ibn.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = IBN(beam, fp);
  fclose(fp);
  return res;
}

int runIB(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_ib.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = IB(beam, 1, fp);
  fclose(fp);
  return res;
}

int runIBSC(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_ibsc_%d_%d.out", a, beam, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = IBSC(beam, step, fp);
  fclose(fp);
  return res;
}

int runItBC(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_itbc_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = ItBC(beam, fp);
  fclose(fp);
  return res;
}
/*
int runSMA(const char *a, int MAX) {
  char fname[100];
  sprintf(fname, "%s_sma.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = SMA(MAX, fp);
  fclose(fp);
  return res;
}
*/

int runAPP(const char *a) {
  char fname[100];
  sprintf(fname, "%s_app.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = APP(fp);
  fclose(fp);
  return res;
}

int runAPS(const char *a) {
  char fname[100];
  sprintf(fname, "%s_aps.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = APS(fp);
  fclose(fp);
  return res;
}

int runAPA(const char *a) {
  char fname[100];
  sprintf(fname, "%s_apa.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = APA(fp);
  fclose(fp);
  return res;
}

int runAPI(const char *a, int i) {
  char fname[100];
  sprintf(fname, "%s_api_%d.out", a, i);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = API(fp, i);
  fclose(fp);
  return res;
}

int runAPGS(const char *a, int B, int M, int P) {
  char fname[100];
  sprintf(fname, "%s_apgs_%d_%d_%d.out", a, B, M, P);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  int res = APGS(fp, B, M, P);
  fclose(fp);
  return res;
}

int runAnytimeContract(const char *a, int MINC, int MAXC, int refBeam, int step) {
  char fname[100];
  sprintf(fname, "%s_anytimecontract_%d_%d_%d.out", a, MINC, MAXC, step);
  if (printName) {
    printf("%s\n", fname);
  }
  int res = 99999;
  FILE* fp;  
  fp = fopen(fname, "a");
  res = anytimeContractSearch(MINC, MAXC, MAX_D, fp, refBeam, step);
  fclose(fp);
  return res;
}


//---------------------------------------------------------------------------------

#include "run_tests.cxx"

void runAll(const char *fname) {    
  printName = 1;
  //runBeam(fname, 1200);
  //runBeam1(fname, 1200);/*
  //runItB(fname, 1000, 1);
  //runAWAS(fname);/*
  //runAWA(fname);
  //runANA(fname);
  //runBS(fname, 500);
  /*
  //runDFBB(fname);/*
  //runAPP(fname);/*
  //runAPA(fname);/*
  runAPS(fname);/*
  runAPI(fname, 50);/*
  runACI(fname, 100);/*
  runIBSIM(fname, 500, 1);/*
  runACA(fname);/*
  for (int i = 14; i <= 20; i++) {
	runBeam(fname, i * (i + 1) / 2);
  }
  /*
  runIBS(fname, 300, 1);
  runItB(fname, 300, 1);
  runAWA(fname);
  runANA(fname);
  runBS(fname, 50);
  runDFBB(fname);*/
  /*
  runPAWA(85, fname);
  runPAWA(90, fname);
  runPAWA(95, fname);
  runPAWA(100, fname);*/
  //runIBSA(fname, 1000, 1);
  //runItBC(fname, 66);
  //OPT = runAStar(fname);
 
  //runAWA(fname);
  //runARA(2.0, 0.1, fname);
  
  //int inc = 10;
  /*
    runIB(fname, 300);
    runIBN(fname, 300);
  for (int beam = 50; beam < 301; beam += 50) {
    //runBS(fname, beam);
    runBeam(fname, beam);
  }*
  runIBS(fname, 200, 1);
  runItB(fname, 200, 1);
  runBS(fname, 100);
  runBS(fname, 200);
  runBS(fname, 300);*/
  //runItBB(fname, 1000, 1);
  //runBS(fname, 60);
  //runBS(fname, 150);
  //runItBB(fname, 1000, 1);
  //runDFBB(fname);
  //runSMA(fname, 100000);
  //runIBSN(fname, 150);
  //runIBS(fname, 200, 1);
  //runItBC(fname, 50);
  
  int MINC = (MAX_D+1);
  int MAXC = 10000*MINC;
  int step = MAX_D+1;
  runAnytimeContract(fname, MINC, MAXC, 1000000, step);
}
/*
void runAll1(const char *fname) {    
  printName = 1;
  runIBS(fname, 200, 1);
  runItB(fname, 200, 1);
  runBS(fname, 200);
}
*/
void printCopyright() {
  printf("*****************************************\n");
  printf("* Copyright:\n");
  printf("* Satya Gautam Vadlamudi\n");
  printf("* Sunandita Patra\n");
  printf("* IIT Kharagpur\n");
  printf("*****************************************\n\n");
}

int main() {
  printCopyright();/*
  MAX_D = 500;
  _size = 4;
  char a[100];
  for (int i = 17; i <= 100; i++) {
	sprintf(a, "input_benchmarks/puz15/%d", i);
    read(a);
	sprintf(a, "output_benchmarks/puz15/%d", i);
    runAll(a);
    reset();
    if (i == 18) {
	  i = 25;
	}
  }
  /*/
  //runOldRandomTests();
  //runNewRandomTests();
  
  MAX_D = 1000;
  _size = 5;
  char a[100];
  int input;
  printf("mode : ");
  scanf("%d", &mode);
  scanf("%d",&input);
  int end;
  scanf("%d",&end);
  for (int j = input; j <= end; j++) {
	int i = j ;
	sprintf(a, "input_benchmarks/puz24/%d", i);
    read(a);
	sprintf(a, "output_benchmarks/puz24_%d/%d", mode, i);
    runAll(a);
    reset();
  }
/*
  //------------------------------------------------
// not really the easy ones.. but Table III ones
  read("testcases/benchmarks/easy/1");
  OPT = 95;
  runAll("testcases/benchmarks/easy/1");
  reset();

  read("testcases/benchmarks/easy/4");
  OPT = 98;
  runAll("testcases/benchmarks/easy/4");
  reset();

  read("testcases/benchmarks/easy/5");
  OPT = 100;
  runAll("testcases/benchmarks/easy/5");
  reset();

  read("testcases/benchmarks/easy/6");
  OPT = 101;
  runAll("testcases/benchmarks/easy/6");
  reset();

  read("testcases/benchmarks/easy/8");
  OPT = 108;
  runAll("testcases/benchmarks/easy/8");
  reset();
  
  //------------------------------------------------

  read("testcases/benchmarks/hard/9");
  OPT = 113;
  runAll("testcases/benchmarks/hard/9");
  reset();

  read("testcases/benchmarks/hard/10");
  OPT = 114;
  runAll("testcases/benchmarks/hard/10");
  reset();

  read("testcases/benchmarks/hard/11");
  OPT = 106;
  runAll("testcases/benchmarks/hard/11");
  reset();

  read("testcases/benchmarks/hard/14");
  OPT = 111;
  runAll("testcases/benchmarks/hard/14");
  reset();

  read("testcases/benchmarks/hard/50");
  OPT = 113;
  runAll("testcases/benchmarks/hard/50");
  reset();
  */
  return 0;
}
