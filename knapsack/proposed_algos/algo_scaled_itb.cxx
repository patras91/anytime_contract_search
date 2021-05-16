/*
 * Author: Sunandita Patra
 * email: sunandita.patra@cse.iitkgp.ernet.in
*/

//iterative beam search for knapsack
//giving segmentation faults
int scaled_itBeam(int BEAM, FILE* fp) {   
  time_t start_time = time(NULL);
   
  int result = 0;
  int beam;
  for (beam = 1; beam <= BEAM; beam++) {
	result = BeamR(beam, result, fp, start_time);
    
    if (int(time(NULL) - start_time) > MAX_T) {
      break;
	}  
  }
  fprintf(fp, "%d %d %d\n", beam - 1, result, int(time(NULL) - start_time));
  fflush(fp);
	  
  return result;
}

