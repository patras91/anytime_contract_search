/*
 * Author: Sunandita Patra
 * email: sunandita.patra@gmail.com
 */

int scaled_best_K_awaStar (KNState * state, int k, FILE *fp) {
  //~ printf("\n In best k AWA* ");
  time_t start_time;
  time_t end_time;
  
  start_time = time(NULL);
  k = 1;
  myPriorityQueue *q = new myPriorityQueue (0);
  myPriorityQueue *sus = new myPriorityQueue (0);

  int w = 0;
  int topLevel = 0;
  int level = 0;
  q->push (state);
  int res = -1;
  while (1) {
    topLevel = -1;
    while (!q->empty ()) {
	  KNState *s = (KNState *) q->top ();
	  q->pop ();
	  level = s->GetDepth();
	  if (s->GetFn () <= res) {
	    delete s;
	    k = 1;
	    end_time = time(NULL);
	    fprintf(fp, "%d %d %d\n", w, res, int(end_time - start_time));
	    fflush(fp);
	    printf("w = %d, res = %d time = %d \n", w, res, int(end_time - start_time));
	    break;
	  }
	  if (level > topLevel) {
        topLevel = level;
	  }
	  else if (level <= topLevel - w) {
        sus->push (s);
	    continue;
	  }
	  if (s->IsGoalState()) {
	    if (res < s->GetFn()) {
		  k = 1;
		  res = s->GetFn();
	      end_time = time(NULL);
	      fprintf(fp, "%d %d %d\n", w, res, int(end_time - start_time));
	      fflush(fp);
	      printf("w = %d, res = %d time = %d \n", w, res, int(end_time - start_time));		  
		}
        delete s;
	    break;
	  }
	  GenerateChildren_k_best_AStar (s, q, res);
	  while(q->size()>k) {
	    KNState *t = q->popLast();
	    sus->push(t);
	  }
	  delete s;
	}
	k++;
    //printf("w = %d, res = %d time = %d \n", w, res, int(end_time - start_time));		    
    if (sus->empty () || int(time(NULL) - start_time) > MAX_T) {
	  break;
	} else {
	  w++;
	  while (!q->empty ())
	    {
	      KNState *s = (KNState *) q->top ();
	      q->pop ();
	      delete s;
	    }
	while (!sus->empty() && q->size()<k) {
      KNState *t = sus->popLast();
      q->push(t);
    }
	}
  }

  while (!sus->empty ()) {
    KNState *s = (KNState *) sus->top ();
    sus->pop ();
    delete s;
  }
  delete sus;

  while (!q->empty ()) {
    KNState *s = (KNState *) q->top ();
    q->pop ();
    delete s;
  }
  delete q;
  end_time = time(NULL);
  fprintf(fp, "%d %d %d\n", w, res, int(end_time - start_time));
  printf("w = %d, res = %d time = %d \n", w, res, int(end_time - start_time));  
  fflush(fp);
  
  return res;
}


