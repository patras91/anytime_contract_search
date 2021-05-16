/**
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
*/

void testAll(const char *fname) {    
  OPT = runAStar(fname);
  int res;/*
  
  res = runIBSC(fname, 3);
  assert(res == OPT);
  *
  res = MAX_RES;
  for (int i = 1; i < 5; i++) {
    double t = runIBSNb(fname, i);    
    assert(t <= res);
    res = t;
  }
  *
  res = runItB(fname, 5);
  double t = runIBSNbg(fname, 5);
  assert(t == res);
/*
  //runABS(fname, 3);
/*/
  res = runANA(fname);
  assert(res == OPT);
  /*
  //res = runARA(1.3, 0.1, fname);
  //assert(res == OPT);
  */
  res = runAWA(fname);
  assert(res == OPT);
  
  res = runBS(fname, 1);
  assert(res == OPT);
  
  res = runBS(fname, 2);
  assert(res == OPT);
  
  res = runDFBB(fname);
  assert(res == OPT);
  
  res = runACA(fname);
  assert(res == OPT);
  
  res = runACP(fname);
  assert(res == OPT);
  
  res = runACS(fname);
  assert(res == OPT);
  
  res = runACI(fname, 2);
  assert(res == OPT);
  
  res = runABS(fname, 3);
  assert(res >= OPT);
  
  int res1 = runABS(fname, 4);
  assert(res >= res1);
  assert(res1 >= OPT);
  /*
  res = runIBS(fname, 3, 1);
  assert(res >= OPT);
  */
  res = runBeam(fname, 30);
  assert(res >= OPT);
  
  res1 = runItB(fname, 30, 1);
  assert(res1 <= res);
  /*
  res = runItBB(fname, 3, 1);
  assert(res == res1);
  */
  res = runAPP(fname);
  assert(res == OPT);
  
  res = runAPS(fname);
  assert(res == OPT);
  
  res = runAPA(fname);
  assert(res == OPT);
  
  res = runAPI(fname, 1);
  assert(res == OPT);
  
  res = runAPGS(fname, 1, 2, 16);
  assert(res == OPT);
  
  res = runIBS(fname, 3, 1);
  assert(res >= OPT);
  
  res1 = runIBS(fname, 4, 1);
  assert(res >= res1);
  assert(res1 >= OPT);
  /*
  res = runSMA(fname, nodes);
  assert(res == OPT);
  
  res = runMAWA(fname, 1);
  assert(res == OPT);
  
  res = runMAWAT(fname, 1);
  assert(res == OPT);

  //runIWS(fname, 3);
  runItB(fname, 3);*/
  //runItBS(fname, 3);
  //runItMAWA(fname, 3);
}
/*
void runOldRandomTests() {
  printf("-----------------------------------------\n");
  printf("EXECUTING SOME OLD RANDOM TESTS...\n");
  printName = 0;
  nodes = 15;
  for (int k = 0; k < 100; k++) {
	char fname[100];/*
    sprintf(fname, "testcases/random/%d/in%d.tsp", nodes, k);
    FILE* fp;
    fp = fopen(fname, "w");
    double boundary = 10;
    CreateNodes(fp, boundary);
    fclose(fp);
    *
    sprintf(fname, "testcases/random/%d/in%d", nodes, k);
    read(fname, 0, 0);
    
    testAll(fname);
  }
  printf("SUCCESSFUL!\n");
}
*/
void runNewRandomTests() {
  printf("-----------------------------------------\n");
  printf("EXECUTING SOME NEW RANDOM TESTS...\n");
  printName = 0;
  srand(time(0));
  nodes = 15;  
  for (int k = 0; k < 200; k++) {
	char fname[100];
    sprintf(fname, "testcases/random/%d/in%d.tsp", nodes, k);
    FILE* fp;
    fp = fopen(fname, "w");
    double boundary = 10;
    CreateNodes(fp, boundary);
    fclose(fp);
    
    sprintf(fname, "testcases/random/%d/in%d", nodes, k);
    read(fname, 0, 0);
    
    testAll(fname);
    printf("%d\n", k);
    for (int i = 0; i < nodes - 1; i++) {
	  delete dist[i];
    }
    delete dist;
  }
  printf("SUCCESSFUL!\n");
}
/*
void runBenchmarkTests() {
  printf("-----------------------------------------\n");
  printf("EXECUTING SOME BENCHMARK TESTS...\n");
  printName = 0;

  read("testcases/benchmarks/burma14/burma14", 1, 0);
  OPT = 3323;
  testAll("testcases/benchmarks/burma14/burma14");
  
  printf("SUCCESSFUL!\n");
}
*/
