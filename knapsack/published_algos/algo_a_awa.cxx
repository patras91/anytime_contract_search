
void GenerateChildren_AStar(KNState * state, PriorityQueue * q, int res) {
  for (int i = state->GetTopIndex() + 1; i < num_items; i++) {
    if (state->IsChildPossible(i)) {
   	  KNState *childstate = state->getChild(i);
      //~ childstate->print();
      if (childstate->GetFn() > res) {
   	    q->push (childstate);
      } else {
        delete childstate;
      }
   	}
  }
}

int AStar(KNState* start, FILE *fp) {
  time_t start_time;
  time_t end_time;
  
  start_time = time(NULL);
  
  PriorityQueue *q = new PriorityQueue(myPriorityQueue::state_comp);
  q->push(start);
  int res = 0;
  while(!q->empty ()) {
    KNState *s = (KNState *) q->top();
    //~ printf("\n s:");
    //~ s->print();
    //~ printf("\n state w = %d n = %d ",s->GetWeight(), s->_num_items);
    q->pop();
    if (s->IsGoalState()) {
      res = s->GetFn ();
      printf("\n res = %d",res);
      s->print();
      delete s;
      break;
    }
    GenerateChildren_AStar (s, q, res);
    delete s;
  }
    
  while (!q->empty()) {
    KNState *s = (KNState *) q->top ();
    q->pop ();
    delete s;
  }
  delete q;
  fprintf(fp,"%d %d\n", res, int(time(NULL) - start_time));
  fflush(fp); 
  printf("\n time = %d ",int(time(NULL) - start_time));
  return res;
}

int AStar_Window (KNState * state, FILE *fp) {
  PriorityQueue *q = new PriorityQueue (myPriorityQueue::state_comp);
  PriorityQueue *sus = new PriorityQueue (myPriorityQueue::state_comp);

  time_t start_time = time(NULL);
  int w = 0;
  int topLevel = 0;
  int level = 0;
  q->push (state);
  int res = 0;
  while (1) {
    topLevel = -1;
    while (!q->empty ()) {
      KNState *s = (KNState *) q->top ();
      q->pop ();
      level = s->GetDepth();
      if (s->GetFn () <= getLim()) {
        delete s;
        printf("w = %d, res = %d %ld\n", w, getRes(), (time(NULL)-start_time));
        fprintf(fp, "%d %d %ld\n", w, getRes(), (time(NULL)-start_time));
        fflush(fp);
        break;
      }
      if (level > topLevel) {
        topLevel = level;
      } else if (level <= topLevel - w) {
        sus->push (s);
        continue;
      }
      if (s->IsGoalState()) {
        if (getLim() < s->GetFn()) {
          insertInList(s);
        }
        printf("w = %d, res = %d %ld\n", w, getRes(), (time(NULL)-start_time));
        fprintf(fp, "%d %d %ld\n", w, getRes(), (time(NULL)-start_time));
        fflush(fp);
        delete s;
        break;
      }
      GenerateChildren_AStar (s, q, getLim());
      delete s;
    }
      
    if (sus->empty ()) {
      break;
    } else {
      if(time(NULL) - start_time > MAX_T) {
        printf("w = %d, res = %d %ld\n", w, getRes(), (time(NULL)-start_time));
        fprintf(fp, "%d %d %ld\n", w, getRes(), (time(NULL)-start_time));
        break;
      }
      w++;
      while (!q->empty ()) {
        KNState *s = (KNState *) q->top ();
        q->pop ();
        delete s;
      }
      PriorityQueue* t = q;
      q = sus;
      sus = t;
    } 
    if(time(NULL) - start_time > MAX_T) {
      printf("w = %d, res = %d\n", w, getRes());
      fprintf(fp, "%d %d %ld\n", w, getRes(), (time(NULL)-start_time));
      break;
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
  return getRes();
}
