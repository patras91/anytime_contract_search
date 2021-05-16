/**
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
*/

void testAll(const char *fname) {
  int res;
  OPT = runAStar(fname);
  /*
  res = runIBSC(fname, 5, 1);
  assert(res == OPT);*
  */
  res = runAWA(fname);
  assert(res == OPT);
  
  res = runAWAS(fname);
  assert(res == OPT);
  
  res = runANA(fname);
  assert(res == OPT);
  /*
  res = runARA(1.3, 0.1, fname);
  assert(res == OPT);
  */
  res = runBS(fname, 1);
  assert(res == OPT);
  res = runBS(fname, 2);
  assert(res == OPT);
  /*
  res = runMAWA(fname, MAX_D);
  assert(res == OPT);
  */
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
  
  res = runBeam(fname, 10);
  assert(res <= MAX_D);
  assert(res >= OPT);
  /*
  double res1 = runItBB(fname, 5, 1);
  assert(res1 <= res);
  assert(res1 >= OPT);
  */
  int res1 = runItB(fname, 10, 1);
  assert(res1 <= res);
  assert(res1 >= OPT);
  /*
  res = runBeam1(fname, 10);
  assert(res <= MAX_D);
  assert(res >= OPT);
  res1 = runItB1(fname, 10, 1);
  assert(res1 <= res);
  assert(res1 >= OPT);
  /*
  res1 = runIBS(fname, 5, 1);
  assert(res1 <= MAX_D);
  assert(res1 >= OPT);
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
  
  res = runIBS(fname, 5, 1);
  res1 = runIBS(fname, 10, 1);
  assert(res1 <= res);
  res = runIBS(fname, 15, 1);
  assert(res <= res1);
  
  res = runIBSF(fname, 5, 1);
  res1 = runIBSF(fname, 10, 1);
  assert(res1 <= res);
  res = runIBSF(fname, 15, 1);
  assert(res <= res1);
  
  res = runIBSI(fname, 5, 1);
  res1 = runIBSI(fname, 10, 1);
  assert(res1 <= res);
  res = runIBSI(fname, 15, 1);
  assert(res <= res1);
  
  res = runIBSIM(fname, 5, 1);
  res1 = runIBSIM(fname, 10, 1);
  assert(res1 <= res);
  res = runIBSIM(fname, 15, 1);
  assert(res <= res1);
  //assert(res < MAX_D);
}

void runOldRandomTests() {
  printf("-----------------------------------------\n");
  printf("EXECUTING SOME OLD RANDOM TESTS...\n");
  printName = 0;
  int size = 3;  
  MAX_D = 100;
  for (int k = 100; k < 200; k++) {
	char fname[100];
    sprintf(fname, "testcases/random/%d/%d", size, k);    
    read_test(fname);
    testAll(fname);
    printf("%d\n", k);
    reset();
  }
  printf("ALL SUCCESSFUL!\n");
}

void runNewRandomTests() {
  printf("-----------------------------------------\n");
  printf("EXECUTING SOME NEW RANDOM TESTS...\n");
  printName = 0;
  srand(time(0));
  int size = 3;  
  MAX_D = 100;
  for (int k = 0; k < 100; k++) {
	char fname[100];
    sprintf(fname, "testcases/random/%d/%d.puz", size, k);
    FILE* fp = fopen(fname, "w");
    Create(size, fp);
    fclose(fp);
    sprintf(fname, "testcases/random/%d/%d", size, k);    
    //read_test(fname);
    testAll(fname);
    printf("%d\n", k);
    reset();
  }
  printf("ALL SUCCESSFUL!\n");
}
/*
void runBenchmarkTests() {
  printf("-----------------------------------------\n");
  printf("EXECUTING SOME BENCHMARK TESTS...\n");
  printName = 0;
  read_test("testcases/benchmarks/burma14/burma14", 1, 0);
  testAll("testcases/benchmarks/burma14/burma14");
  printf("SUCCESSFUL!\n");
}
*/
