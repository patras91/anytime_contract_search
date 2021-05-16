#include <stdio.h>
#define NUM_PUZ 50

int main() {
	
  double sum = 0;
  int count = 0;
  for(int i = 0; i<NUM_PUZ; i++) {
    char fname[500];
    
    for(int j=1;j<=3;j++) {
      sprintf(fname, "output_benchmarks/puz24_%d/%d_anytimecontract_1001_10010000_1001.out",j,i+1);
      printf("%s\n",fname);
      FILE *f = fopen(fname, "r");    
      while (!feof(f)) {
	    long exp;
	    int time;
	    fscanf(f, "%*d %*d %*lf %ld %*ld %d", &exp, &time);
	    if(time!=0) {
	      sum = sum + exp*1.0/time;
	      count ++;
	    }
      }
      fclose(f);
    }
  }
  double avg_rate;
  if(count) {
    avg_rate = sum/count;
  } else {
	avg_rate = -1;
  }
    
  char rateFile[100];
  sprintf(rateFile, "rate/rate.txt");
  FILE *f1 = fopen(rateFile,"w");
  fprintf(f1, "%lf",avg_rate);
  fclose(f1);
  return 0;
}

