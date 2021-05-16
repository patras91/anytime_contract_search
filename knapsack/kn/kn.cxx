// Code for kn problem.
// Author: satya@cse.iitkgp.ernet.in
//         sunandita.patra@cse.iitkgp.ernet.in
#define MAX_T 12000
#define BOUNDARY 100
#define numK 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <stdio.h>
#include <list>
#include <ctime>
#include <stack>

int lib;
int reslist[numK];
int sum;

int num_items;
int* value;
int* weight;
int weight_limit;

// For fn overestimate computation, max val/wt first
int*  sorted_value;
int*  sorted_weight;
int* sorted_index;

//~ #include "kn_state.h"
#include "kn_state.cxx"

KNState **stateList;
#include "../library/my_priority_queue.h"
#include "../library/my_priority_queue.cxx"

using namespace std;
#define PriorityQueue priority_queue<KNState*, vector<KNState*>, int (*)(KNState*, KNState*)>
//---------------------------------------------------------------------------------

KNState* CreateInitState() {
  KNState *state = new KNState(num_items);
  state->SetDepth(0);
  state->SetWeight(0);
  state->SetParent(NULL);
  state->SetGn(0);
  state->SetFnOverEstimate();
  return state;
}

void insertInList(KNState *state) {
  int i = 0 ;
  int res = state->GetFn();
  int zero = reslist[0];
  if(resSubSet(stateList, state)) {
    return;
  }
  while(reslist[i] < res && i < numK) {
    if(i == 0) {
      i++;
      continue;
    }
    reslist[i-1] = reslist[i];
    copy(stateList[i-1], stateList[i]);
    i++;
  }
  if(i == 0) {
    return;
  }
  reslist[i-1] = res;
  copy(stateList[i-1], state);
  sum += res - zero;
}

int getLim() {
  return reslist[0];
}
int getRes() {
  //~ printf("\n sum = %d\n", sum);
  return sum;
}

void initK() {
  for(int i = 0; i<numK; i++) {
    reslist[i] = 0;
    stateList[i]->SetFn(0.0);
  }
  sum = 0;
}
int K;
double current_top_K_size_sum;
myPriorityQueue* topksize_result;

//---------------------------------------------------------------------------------
#include "../published_algos/algo_a_awa.cxx"    
//~ #include "../published_algos/algo_ara.cxx"
#include "../published_algos/algo_sma.cxx"
//~ #include "../published_algos/algo_mawa.cxx"
//~ #include "../published_algos/algo_mak.cxx"
//#include "../published_algos/algo_beam_stack.cxx"
#include "../published_algos/algo_beam.cxx"
#include "../published_algos/algo_dfbb.cxx"
#include "../published_algos/algo_contract_search.cxx"
//-------------------------------------------------------------------------------
#include "../proposed_algos/algo_best_K_awaStar.cxx"
#include "../proposed_algos/algo_restricted_open.cxx"
#include "../proposed_algos/algo_aro.cxx"
#include "../proposed_algos/algo_abeam.cxx"
#include "../proposed_algos/algo_scaled_best_K_awa_star.cxx"
#include "../proposed_algos/algo_scaled_modified_beam.cxx"
#include "../proposed_algos/algo_itb.cxx"
#include "../proposed_algos/algo_scaled_itb.cxx"
#include "../proposed_algos/algo_anytime_contract_search.cxx"

void CreateNodes(const char *fname, int boundary) {
  int value, weight, sw = 0;
  FILE* fp = fopen(fname, "w");
  fprintf(fp, "%d\n", num_items);  // , int((0.4 + 0.1 * (rand() % 3)) * 0.5 * num_items * boundary));
  for (int j = 0; j < num_items; j++) {
    value = rand() % (boundary - 1) + 1;
    weight = rand() % (boundary - 1) + 1;
    sw += weight;
    fprintf(fp, "%d\t%d\t%d\n", j + 1, weight, value);
  }
  fprintf(fp, "%d\n", sw);
  fclose(fp);
}

void ReadInput(const char *fname) {
  FILE* fp = fopen(fname, "r");
  
  fscanf(fp, "%d", &num_items);
  //fscanf(fp, "%d", &weight_limit);
  
  value = new int[num_items];
  weight = new int[num_items];  
  sorted_value = new int[num_items];
  sorted_weight = new int[num_items];
  sorted_index = new int[num_items];
  int t;
  for (int m = 0; m < num_items; m++) {
	if(lib == 0) 
    fscanf(fp, "%d", &t);
    fscanf(fp, "%d", &weight[m]);
    fscanf(fp, "%d", &value[m]);
    sorted_value[m] = value[m];
    sorted_weight[m] = weight[m];
    sorted_index[m] = m;
  }
  fscanf(fp, "%d", &weight_limit);
  if(lib == 0)
  weight_limit /= 2;
  fclose(fp);
  
  for (int i = 0; i < num_items; i++) {
	//~ float max = (sorted_value[i] + 1.0) / sorted_weight[i];
	float max = (sorted_value[i]*1.0) / sorted_weight[i];
	int max_index = i;
	for (int j = i + 1; j < num_items; j++) {
	  //~ if (max < (sorted_value[j] + 1.0) / sorted_weight[j]) {
	  if (max < (sorted_value[j]*1.0) / sorted_weight[j]) {
		//~ max = (sorted_value[j] + 1.0) / sorted_weight[j];
		max = (sorted_value[j]*1.0) / sorted_weight[j];
		max_index = j;
	  }
	}
	t = sorted_value[i];
	sorted_value[i] = sorted_value[max_index];
	sorted_value[max_index] = t;
	
	t = sorted_weight[i];
	sorted_weight[i] = sorted_weight[max_index];
	sorted_weight[max_index] = t;
	
	t = sorted_index[i];
	sorted_index[i] = sorted_index[max_index];
	sorted_index[max_index] = t;
  }
  /*
  printf("num_items: %d, weight_limit: %d\n", num_items, weight_limit);
  for (int i = 0; i < num_items; i++) {
	printf("i: %d, v: %d, w: %d\n", i, value[i], weight[i]);
  }  
  printf("sorted list:\n");
  for (int i = 0; i < num_items; i++) {
	printf("i: %d, v: %d, w: %d, v/w: %f\n", sorted_index[i], sorted_value[i], sorted_weight[i], (sorted_value[i] + 1.0) / sorted_weight[i]);
  }
  printf("------------\n");
  */
}

//---------------------------------------------------------------------------------

int runAStar() {
  FILE *fp;
  char fout[100];
  sprintf(fout, "testcases/random/output/astar/%dastar_noutput.txt", num_items);
  fp = fopen(fout, "a");
  int astar = AStar(CreateInitState(),fp);
  printf("astar: %d\n", astar);  
  fclose(fp);
  return astar;
}

int run_AStar_Window () {
  FILE *fp;
  char fout[100];
  sprintf(fout, "testcases/random/output/awas/%dawas_noutput.txt", num_items);
  fp = fopen(fout, "a");
  int awas = AStar_Window(CreateInitState(),fp);
  printf("awas: %d\n", awas);  
  fclose(fp);
  return awas;
}

int runbest_K_awaStar() {
  int k = 7;
  FILE *fp;
  char fout[100];
  sprintf(fout, "testcases/random/output/bestk/%dbestKawa_noutput.txt", num_items);
  fp = fopen(fout, "a");
  int best_k_awa = best_K_awaStar(CreateInitState(),k, fp);
  printf("\n best K awastar: %d\n", best_k_awa); 
  fclose(fp);
  return best_k_awa;
}

int run_scaled_best_K_awaStar() {
  int k = 7;
  FILE *fp;
  char fout[100];
  sprintf(fout, "testcases/random/output/scaled_bestk_awas/%dscaledbestKawa_noutput.txt", num_items);
  fp = fopen(fout, "a");
  int scaled_best_k_awa = scaled_best_K_awaStar(CreateInitState(),k, fp);
  printf("\n scaled_best K awastar: %d\n", scaled_best_k_awa); 
  fclose(fp);
  return scaled_best_k_awa;
}

int runDFBB() {
  char fout[100];
  FILE *fp;
  sprintf(fout, "testcases/random/output/dfbb/%ddfbb_output.txt",num_items);
  fp = fopen(fout, "a");
  int dfbb = DFBB(CreateInitState(), fp);
  printf("\n dfbb result: %d\n", dfbb);
  fclose(fp);
  return dfbb;
}

int runModified_beam() {
  char fout[100];
  int beam = 1;
  FILE *fp;
  sprintf(fout, "testcases/random/output/modified_beam/%dmbeam_output.txt",num_items);
  fp = fopen(fout, "a");
  
  if(fout == NULL) {
    printf("\n Cannot open output file.");
    return -1;
  }
  int mbeam = modified_beam(fp, beam);
  printf("\n modified beam result: %d\n", mbeam);
  fclose(fp);
  return mbeam;
}

int run_anytime_modified_beam() {
  char fout[100];
  int beam = 1;
  FILE *fp;
  sprintf(fout, "testcases/random/output/amb/%damb_output.txt",num_items);
  fp = fopen(fout, "a");
  
  if(fout == NULL) {
    printf("\n Cannot open output file.");
    return -1;
  }
  int am = amb(fp, beam);
  printf("\n anytime modified beam result: %d\n", am);
  fclose(fp);
  return am;
}

int run_scaled_Modified_beam() {
  char fout[100];
  int beam = 5;
  FILE *fp;
  sprintf(fout, "testcases/random/output/scaled_modified_beam/%dscaled_mbeam_output.txt",num_items);
  fp = fopen(fout, "a");
  
  if(fout == NULL) {
    printf("\n Cannot open output file.");
    return -1;
  }
  int mbeam = scaled_modified_beam(fp, beam);
  printf("\n scaled modified beam result: %d\n", mbeam);
  fclose(fp);
  return mbeam;
}

int run_itb() {
  char fout[100];
  int beam = 100;  //maximum limit of beam
  FILE *fp;
  sprintf(fout, "testcases/random/output/iterative_beam/%ditb_output.txt",num_items);
  fp = fopen(fout, "a");
  
  if(fout == NULL) {
    printf("\n Cannot open output file.");
    return -1;
  }
  int itb = itBeam(beam, fp);
  printf("\n iterative beam result: %d\n", itb);
  fclose(fp);
  return itb;
}

int run_scaled_itb() {
  char fout[100];
  int beam = 100;  //maximum limit of beam
  FILE *fp;
  sprintf(fout, "testcases/random/output/scaled_iterative_beam/%ditb_output.txt",num_items);
  fp = fopen(fout, "a");
  
  if(fout == NULL) {
    printf("\n Cannot open output file.");
    return -1;
  }
  int scaled_itb = scaled_itBeam(beam, fp);
  printf("\n Scaled iterative beam result: %d\n", scaled_itb);
  fclose(fp);
  return scaled_itb;
}

int runBeam() {
  char fout[100];
  int bw = 1;  //maximum limit of beam
  FILE *fp;
  sprintf(fout, "testcases/random/output/beam/%dbeam_output.txt",num_items);
  fp = fopen(fout, "a");
  
  if(fout == NULL) {
    printf("\n Cannot open output file.");
    return -1;
  }
  int b = beam(fp, bw);
  printf("\n beam result: %d\n", b);
  fclose(fp);
  return b;
}
int runAnytimeBeam() {
  char fout[100];
  //int bw = 2;  //maximum limit of beam
  FILE *fp;
  sprintf(fout, "testcases/random/output/anytimebeam/%dabeam_output.txt",num_items);
  fp = fopen(fout, "a");
  
  if(fout == NULL) {
    printf("\n Cannot open output file.");
    return -1;
  }
  int b = abeam(fp);
  printf("\n anytime beam result: %d\n", b);
  fclose(fp);
  return b;
}

int runAnytimeContractSearch() {
  char fout[100];
  //int bw = 2;  //maximum limit of beam
  FILE *fp;
  int MIN_C = num_items+1;
  int MAX_C = 100*MIN_C;
  int step = num_items+1;
  sprintf(fout, "testcases/random/output/anytimeContract/%danytime_contract_output_%d_%d_%d.txt",num_items, MIN_C, MAX_C, step);
  fp = fopen(fout, "a");
  
  if(fout == NULL) {
    printf("\n Cannot open output file.");
    return -1;
  }
  int b = AnytimeContractSearch(MIN_C, MAX_C, fp, 1000000000, step);
  printf("\n Anytime Contract Result: %d\n", b);
  fclose(fp);
  return b;
}

void printCopyright() {
  printf("*****************************************\n");
  printf("* Copyright:\n");
  printf("* Satya Gautam Vadlamudi\n");
  printf("* Sunandita Patra\n");
  printf("* IIT Kharagpur\n");
  printf("*****************************************\n\n");
}

void displayAllRes() {
  for(int i = 0; i<numK; i++) {
    stateList[i]->print();
  }
}

void runAll() {
  //~ int best_k_awa = runbest_K_awaStar();
  //~ int scaled_best_k_awa = run_scaled_best_K_awaStar();
  

  //~ int modfd_beam = runModified_beam();
  //~ int scaled_modfd_beam = run_scaled_Modified_beam();
  //~ int itb = run_itb();
  //~ int scaled_itb = run_scaled_itb();
  //~ printf("\n --------------beam---------------\n");
  //~ int beam = runBeam();
  int x;
  printf("\n Which algorithm do you want to run?\n1. Anytime modified beam\n2. Anytime beam\n3. AWA Star\n4. DFBB\n5. A Star\n6. Beam search\n7. Anytime Contract Search");
  scanf("%d",&x);
  if(x == 1) {
    printf("\n -------------Anytime modified beam----------------");
    int amb = run_anytime_modified_beam();
  } else if (x == 2){
    printf("\n --------------anytime beam---------------");
    int beam = runAnytimeBeam();
  } else if(x == 3){
    printf("\n ---------------------A* window----------------------");
    int awa = run_AStar_Window();
  } else if(x == 4){
    printf("\n ---------------------dfbb----------------------");
    int dfbb = runDFBB();
  } else if(x == 5){
    printf("\n ---------------------A star----------------------");
    int dfbb = runAStar();
  } else if(x == 6){
    printf("\n ---------------------Beam search----------------------");
    int dfbb = runBeam();
  } else if(x == 7) {
    printf("\n----------------------Anytime Contract Search----------------");
    int acs = runAnytimeContractSearch();
  }
  //~ displayAllRes();
}
int main() {
	
  printCopyright();
  printf("Enter number of items: ");
  scanf("%d", &num_items);
  stateList = new KNState*[numK];
  for(int i = 0; i<numK; i++) {
    stateList[i] = new KNState(num_items);
  } 
  K = 2;
  topksize_result = NULL;
  srand(0);  
  initK();
  char fname[50];
  int beam = 10;
  for (int i = 6; i < 7; i++) {
    if (i != 5) {
      sprintf(fname, "testcases/random/input/%dinput.txt", num_items);
      CreateNodes(fname, BOUNDARY);
    } else {
      sprintf(fname, "testcases/random/input/kn100_1.txt");
    }
  //~ lib = 1;
  //~ sprintf(fname, "testcases/random/input/kn_t1.txt");
  //~ sprintf(fname, "testcases/random/input/kn_t2.txt");
  //~ sprintf(fname, "testcases/random/input/kn_t3.txt");
  //~ sprintf(fname, "testcases/random/input/kn_t4.txt");
  //~ sprintf(fname, "testcases/random/input/kn_t5.txt");
  //~ sprintf(fname, "testcases/random/input/kn_t6.txt");
  //~ sprintf(fname, "testcases/random/input/kn_t7.txt");
  ReadInput(fname);
  runAll();
  
  delete value;
  delete weight;
  delete sorted_value;
  delete sorted_weight;
  delete sorted_index;
  
  ////assert(astar == awa);
  //assert(awa == mawa);
  //assert(mawa == bs);
  //assert(bs == dfbb);
  //assert(dfbb == ara);
  }

  return 0;
}
