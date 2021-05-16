/*
void KN::BackUp(KNState *s, myPriorityQueue* open) {
  if (!s->Completed()) { return; }
  int _min_child_fn = s->GetMinChildFn();
  //assert(s->GetFn() <= _min_child_fn);
  if (s->GetFn() < _min_child_fn) {
    s->SetFn(_min_child_fn);
    if (s->_list == openListIndex) {
      open->pop(s);
      open->push(s);
    }
    KNState* _parent = s->GetParent();
    if (_parent != NULL) {
      BackUp(_parent, open);
    }
  }
}

void SMDeleteState(KNState* last, myPriorityQueue * open, int *used) {
  last->ResetChildrenParent();
  KNState* parent = last->GetParent();
  if (parent != NULL) {
    parent->ResetExistingChild(last->GetTopItem());
    if (!parent->_list == openListIndex) {
      open->push(parent);
      parent->_list = openListIndex;
    }
  }
  delete last;
  last = NULL;
  *used -= 1;
}

void KN::SMGenerateChildren (KNState * state, myPriorityQueue * open, int *used, int MAX) {
  int node_id = state->GetTopItem ();
  //assert(!state->AllChildrenInMemory());
  int lastChildIndex = state->GetLastChildIndex();
  for (int i = lastChildIndex + 1; ; i++) {
    if (i == num_items) {
      i = 0;
    }
    if (!state->IsChosen(i) && !state->DoesExistChild(i)) {
      KNState *childstate = state->Copy ();  
    	  this->Place (childstate, node_id, i);
    	  childstate->SetDepth(state->GetDepth() + 1);
      //assert(childstate->GetFn() >= state->GetFn());
      state->SetExistingChild(i, childstate);
      BackUp(state, open);
      if (state->AllChildrenInMemory()) {
	open->pop(state);
	state->_list = closedListIndex;		
      }
      *used += 1;
      if (*used > MAX) {
	KNState* last = open->popLastWithNoChild();
	//assert(last->GetDepth() > 0);  // Start node never gets deleted for this problem when there is at least NUM_V node memory.
	SMDeleteState(last, open, used);
      }
      open->push(childstate);
      childstate->_list = openListIndex;
      return;
    }
  }
}

int KN::SMAStar (KNState * start, int MAX, FILE* fp) {
  time_t start_time;
  time_t end_time;
  start_time = time(NULL);
  
  myPriorityQueue *open = new myPriorityQueue (0);
  
  open->push(start);
  start->_list = openListIndex;
  
  int used = 1;
  int res = 9999999;
  while (!open->empty ()) {
    KNState *s = (KNState *) open->top ();
    if (s->IsGoalState ()) {
      res = s->GetFn ();
      end_time = time(NULL);
      sma_t += ((int) (end_time - start_time)) / 1;
      fprintf(fp, "%d %d\n", res, (int)(end_time - start_time));
	  fflush(fp);
	  while (s != NULL) {
		printf("sma path %d %d\n", s->o_f, s->GetDepth());
		s = s->GetParent();
	  }
      break;
    }
    this->SMGenerateChildren (s, open, &used, MAX);
  }

  while (!open->empty ()) {
    KNState *s = (KNState *) open->top ();
    open->pop ();
	SMDeleteState(s, open, &used);
  }
  delete open;
  
  return res;
}
*/
