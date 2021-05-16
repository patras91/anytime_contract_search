#include <stdio.h>

int main() {
  char c = 'a';
  int n;
  int i = 0;
  FILE *fin = fopen("kn_t5.txt","r");
  FILE *fout = fopen("../random/input/kn_t5.txt","w");
  fprintf(fout, "100");
  fflush(fout);
  while(i<10000) {
    while(c!=',') {
      fscanf(fin, "%c",&c);
    }
    fscanf(fin, "%d", &n);
    fprintf(fout, "\n%d ", n);
    fscanf(fin, "%c", &c);
    fscanf(fin, "%d", &n);
    fprintf(fout, "%d", n);
    printf("\n scanned %d ", ++i);
    c = 'a';
  }
  fclose(fin);
  fclose(fout);
}
