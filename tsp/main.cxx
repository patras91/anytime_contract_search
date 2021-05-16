/**
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
 * Sunandita Patra
*/

#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <vector>

using namespace std;
#include <string>
#include <map>

#define MAX_T 3840

int nodes;
int **dist;
#include "read_data.cxx"

int OPT;

#define MAX_RES 999999999

int result_ana;

//#include "tsp_state.h"
#include "tsp_state.cxx"

//#include "library/my_priority_queue.h"
#include "library/my_priority_queue.cxx"
bool Compare(TspState* a, TspState* b, short flag) {
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

TspState* CreateInitState() {
  TspState *state = new TspState(1);
  state->Initialize();
  return state;
}

//---------------------------------------------------------------------------------

long NUM_NODES_EXPANDED;
long CUR_MEM;
long CUR_MAX_MEM;

#include "published_algos/algo_a.cxx"
//#include "published_algos/algo_ara.cxx"
#include "published_algos/algo_ana.cxx"
#include "published_algos/algo_awa_family.cxx"/*
#include "published_algos/algo_sma.cxx"
#include "published_algos/algo_mawa.cxx"
#include "published_algos/algo_mawat.cxx"*/
#include "published_algos/algo_beam_stack.cxx"
#include "published_algos/algo_dfbb.cxx"
//#include "published_algos/algo_beam.cxx"
#include "published_algos/algo_acs_family.cxx"
#include "published_algos/algo_beam_family.cxx"
#include "published_algos/algo_abs.cxx"
#include "published_algos/algo_contract_search.cxx"

//#include "proposed_algos/algo_iws.cxx"
#include "proposed_algos/algo_ibs_family.cxx"
#include "proposed_algos/algo_anytime_contract_search.cxx"
//#include "proposed_algos/algo_ibs_aine.cxx"
//#include "proposed_algos/algo_ibsn.cxx"

//#include "proposed_algos/algo_itbb.cxx"
//#include "proposed_algos/algo_itb.cxx"
//#include "proposed_algos/algo_ibsc.cxx"
//#include "proposed_algos/algo_itbc.cxx"
//#include "proposed_algos/algo_itbs.cxx"
//#include "proposed_algos/algo_itmawa.cxx"
/*
#include "proposed_algos/algo_aco.cxx"
#include "proposed_algos/algo_acp.cxx"
#include "proposed_algos/algo_acs.cxx"
#include "proposed_algos/algo_aca.cxx"
#include "proposed_algos/algo_aci.cxx"
*/
#include "proposed_algos/algo_aps_family.cxx"
/*
#include "proposed_algos/algo_app.cxx"
#include "proposed_algos/algo_aps.cxx"
#include "proposed_algos/algo_apa.cxx"
#include "proposed_algos/algo_api.cxx"
#include "proposed_algos/algo_apgs.cxx"
*/
//#include "proposed_algos/new/algo_size.cxx"
//#include "proposed_algos/new/algo_itsb.cxx"
//#include "proposed_algos/new/algo_ab.cxx"
//#include "proposed_algos/new/algo_as.cxx"
//--------------------------------------------------------------------------------

bool printName;

double runAStar(const char *a) {
  char fname[100];
  sprintf(fname, "%s_a.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = AStar(fp);
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
double runANA(const char *a) {
  char fname[100];
  sprintf(fname, "%s_ana.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ANA(fp);
  fclose(fp);
  return res;
}
/*
double runACO(const char *a) {
  char fname[100];
  sprintf(fname, "%s_aco.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ACO(fp);
  fclose(fp);
  return res;
}
*/
double runACP(const char *a) {
  char fname[100];
  sprintf(fname, "%s_acp.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ACP(fp);
  fclose(fp);
  return res;
}

double runACS(const char *a) {
  char fname[100];
  sprintf(fname, "%s_acs.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ACS(fp);
  fclose(fp);
  return res;
}

double runACA(const char *a) {
  char fname[100];
  sprintf(fname, "%s_aca.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ACA(fp);
  fclose(fp);
  return res;
}

double runACI(const char *a, int I) {
  char fname[100];
  sprintf(fname, "%s_aci_%d.out", a, I);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ACI(I, fp);
  fclose(fp);
  return res;
}
/*
double runARA(double epsilon, double delta, const char *a) {
  char fname[100];
  sprintf(fname, "%s_ara.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ARA(epsilon, delta, fp);
  fclose(fp);
  return res;
}
*/
double runBeam(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_beam.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = Beam(beam, fp);
  fclose(fp);
  return res;
}

double runBS(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_bs_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = Beam_Stack(beam, fp);
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
  double res = DFBB(fp);
  fclose(fp);
  return res;
}
/*
double runMAWA(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_mawa_%db.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = MAWA(beam * nodes, fp);
  fclose(fp);
  return res;
}

double runMAWAT(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_mawat_%db.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = MAWAT(beam * nodes, fp);
  fclose(fp);
  return res;
}

double runSMA(const char *a, int MAX) {
  char fname[100];
  sprintf(fname, "%s_sma.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = SMA(MAX, fp);
  fclose(fp);
  return res;
}
/*
double runIWS(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_iws_%db.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = IWS(beam * nodes, fp);
  fclose(fp);
  return res;
}
*/
double runIBS(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_ibs_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = IBS(beam, step, fp);
  fclose(fp);
  return res;
}/*
double runIBSA(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_ibsa_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = IBSA(beam, fp);
  fclose(fp);
  return res;
}
*/
double runABS(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_abs.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ABS(beam, fp);
  fclose(fp);
  return res;
}
/*
double runIBSN(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_ibsn_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = IBSN(beam, fp);
  fclose(fp);
  return res;
}

double runIBSNb(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_ibsnb_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = IBSNb(beam, fp);
  fclose(fp);
  return res;
}

double runIBSNbg(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_ibsnbg_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = IBSNbg(beam, fp);
  fclose(fp);
  return res;
}

double runIBSC(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_ibsc_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = IBSC(beam, fp);
  fclose(fp);
  return res;
}
*/
double runItB(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_itb_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ItB(beam, step, fp);
  fclose(fp);
  return res;
}
/*
double runItBC(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_itbc_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ItBC(beam, fp);
  fclose(fp);
  return res;
}
*
double runItBB(const char *a, int beam, int step) {
  char fname[100];
  sprintf(fname, "%s_itbb_%d.out", a, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = ItBB(beam, step, fp);
  fclose(fp);
  return res;
}
/*
double runItBS(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_itbs_%d.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = itBS(beam, fp);
  fclose(fp);
  return res;
}

double runItMAWA(const char *a, int beam) {
  char fname[100];
  sprintf(fname, "%s_itmawa_%db.out", a, beam);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = itMAWA(beam * nodes, fp);
  fclose(fp);
  return res;
}
*

double runSize(const char *a, int size) {
  char fname[100];
  sprintf(fname, "%s_size.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = Size(size, fp);
  fclose(fp);
  return res;
}

double runItSB(const char *a, int size) {
  char fname[100];
  sprintf(fname, "%s_itsb_%d.out", a, size);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = itSizeBasic(size, fp);
  fclose(fp);
  return res;
}

double runAB(const char *a) {
  char fname[100];
  sprintf(fname, "%s_ab.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res;
  time_t start_time = time(NULL);
  pid_t pid = fork();
  if (pid == 0) {
    //printf("child process begun\n", pid, status);
    res = AB(fp);
    exit(0);
  } else if (pid > 0) {
    int status;
	if (waitpid (pid, &status, 0) != pid) {
      status = -1;
    }
    if (status == -1) {
      fprintf(fp, "wrong child %d\n", int(time(NULL) - start_time));
    } else if (status == 0) {
      fprintf(fp, "terminated normally in %d seconds\n", int(time(NULL) - start_time));
	} else {
      fprintf(fp, "terminated abnormally in %d seconds\n", int(time(NULL) - start_time));
	}
  } else {
    printf("%d : fork failed\n", pid);
  }
  fclose(fp);
  return res;
}
/*
double runAS(const char *a) {
  char fname[100];
  sprintf(fname, "%s_as.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = AS(fp);
  fclose(fp);
  return res;
}*/

double runAPP(const char *a) {
  char fname[100];
  sprintf(fname, "%s_app.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = APP(fp);
  fclose(fp);
  return res;
}

double runAPS(const char *a) {
  char fname[100];
  sprintf(fname, "%s_aps.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = APS(fp);
  fclose(fp);
  return res;
}

double runAPA(const char *a) {
  char fname[100];
  sprintf(fname, "%s_apa.out", a);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = APA(fp);
  fclose(fp);
  return res;
}

double runAPI(const char *a, int I) {
  char fname[100];
  sprintf(fname, "%s_api_%d.out", a, I);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = API(fp, I);
  fclose(fp);
  return res;
}

double runAPGS(const char *a, int B, int M, int P) {
  char fname[100];
  sprintf(fname, "%s_apgs_%d_%d_%d.out", a, B, M, P);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = APGS(fp, B, M, P);
  fclose(fp);
  return res;
}

double run_anytime_contract_search(const char *a, int MIN_C, int MAX_C, int refBeam, int step) {
  printf("\n Running Anytime Contract Search\n");
  char fname[100];
  sprintf(fname, "%s_anytime_contract_search_%d_%d_%d.out", a, MIN_C, MAX_C, step);
  if (printName) {
    printf("%s\n", fname);
  }
  FILE* fp;  
  fp = fopen(fname, "a");
  double res = AnytimeContractSearch(a, MIN_C, MAX_C, fp, refBeam, step);
  fclose(fp);
  printf("\n %s",fname);
  return res;
}

//---------------------------------------------------------------------------------

#include "run_tests.cxx"

int beam;

void runAll(const char *fname1) {    
	char fname[100];
	sprintf(fname, "output_benchmarks/%s", fname1);
  printName = 1;
  //runAPI(fname, 100);/*
  //runAPP(fname);/*
  //runAPS(fname);/*
  //runACP(fname);
  /*
  //runAPA(fname);
  //runANA(fname);
  runAWA(fname);
  //runBS(fname, 100);
  //runDFBB(fname);
  //runItB(fname, beam, 1);
  //runIBS(fname, beam, 1);
  //runABS(fname, 100);
/*
  runIBS(fname, beam, 1);
  runAWA(fname);
  runANA(fname);
  runABS(fname, beam);*
  runBS(fname, 50);
  runDFBB(fname);
  //runItB(fname, beam, 1);
  //runItBB(fname, beam, 1);
  
  //runAStar(fname);
  /*
  runItB(fname, beam, 1);
  runItBB(fname, beam, 1);
  runBS(fname, 10);
  runDFBB(fname);
  runIBS(fname, beam, 1);
  /*
  runANA(fname);
  runDFBB(fname);
  runItB(fname, beam);
  runIBS(fname, beam);
  runABS(fname, beam);
  if (beam < 5) {
    runBS(fname, beam);
  } else if (nodes > 1000) {
    runBS(fname, 5);
  } else {
    runBS(fname, 10);
  }
  //runAB(fname);
  /*
  runABS(fname, 100);
  runIBS(fname, 100);
  runItB(fname, 100);
  runBS(fname, 50);*/
  //OPT = runAStar(fname);
 
  //runANA(fname);
  //runAWA(fname);
  //runARA(2.0, 0.1, fname);
  
  //runItB(fname, 1000);
  /*runIBSA(fname, 60);
  runItB(fname, 60);
  runBS(fname, 25);
  runBS(fname, 50);
  runBS(fname, 75);
  runBS(fname, 100);
  //runItB(fname, 60);
  //runIBSC(fname, 5);
  /*
  for (int beam = 15; beam < 20; beam += 5) {
    //runBeam(fname, beam);
    runBS(fname, beam);
    //runMAWAT(fname, beam);
  }
  //runDFBB(fname);
  //runSMA(fname, 100000);

  //runIWS(fname, MAX_BEAM * nodes);
  //runItBB(fname, 10000);
  //runItBS(fname, MAX_BEAM);
  //runItMAWA(fname, MAX_BEAM * nodes);
  * */
  
  int MIN_C = nodes + 1 ;
  int MAX_C = 1000*MIN_C;
  int step = MIN_C;
  run_anytime_contract_search(fname, MIN_C, MAX_C, 100000000, step);
  
  for (int i = 0; i < nodes - 1; i++) {
	delete dist[i];
  }
  delete dist;
}

void printCopyright() {
  printf("*****************************************\n");
  printf("* Copyright:\n");
  printf("* Satya Gautam Vadlamudi\n");
  printf("* IIT Kharagpur\n");
  printf("*****************************************\n\n");
}

int main() {
  printCopyright();
  
  //runBenchmarkTests();
  //runOldRandomTests();
  //runNewRandomTests();
  
  //------------------------------------------------
  #include "tsp_list.cxx"
  
  //read("pla85900");
  return 0;
}
