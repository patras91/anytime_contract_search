
void MWBackUp(KNState *s, myPriorityQueue* open, myPriorityQueue* sus) {
  if (!s->Completed()) { return; }
  int _max_child_fn = s->GetMaxChildFn();/*
  if (_max_child_fn == -1) {
	return;
  }*/
  //assert(s->GetFn() >= _max_child_fn);
  if (s->GetFn() > _max_child_fn) {
    s->SetFn(_max_child_fn);
    if (s->_list == openListIndex) {
      open->pop(s);
      open->push(s);
    }
    if (s->_list == susListIndex) {
      sus->pop(s);
      sus->push(s);
    }
    KNState* _parent = s->GetParent();
    if (_parent != NULL) {
      MWBackUp(_parent, open, sus);
    }
  }
}

void DeleteState(KNState* s, int* used, myPriorityQueue* open, myPriorityQueue* sus, myPriorityQueue* processed) {
  s->ResetChildrenParent();
  KNState* parent = s->GetParent();
  if (parent != NULL) {
    parent->ResetExistingChild(s->GetTopItem());
    if (parent->_list == closedListIndex) {
      if (s->_list == susListIndex) {
        sus->push(parent);
        parent->_list = susListIndex;
      } else if (s->_list == openListIndex) {
        open->push(parent);
        parent->_list = openListIndex;		
	  } else if (!parent->HasChildren()) {  // s in processsed list
		processed->push(parent);
		parent->_list = processedListIndex;
      }
    }
  }
  delete s;
  s = NULL;
  *used -= 1;
}

short IsRootTraceable(KNState* p) {
  KNState* t = p;
  while(t->GetParent() != NULL) {
	if (t->_list == susListIndex) {
      return 1;
	}
    t = t->GetParent();
  }
  return t->GetDepth() == 0;
}

// Ensures tree structure of sus.
void mPushToSus(KNState* s, int* used, myPriorityQueue* open, myPriorityQueue* sus, myPriorityQueue* processed) {
  if (IsRootTraceable(s)) {
    sus->push(s);
    s->_list = susListIndex;
  } else {
    DeleteState(s, used, open, sus, processed);
  }
}

void MAWGenerateChildren (KNState *state, myPriorityQueue *open, myPriorityQueue *sus, myPriorityQueue* processed, int* used, int MAX, int res) {
  //assert(!AllChildrenInMemory(state));
  int lastChildIndex = state->GetLastChildIndex();
  for (int i = lastChildIndex + 1; ; i++) {
    if (i == num_items) {
      i = 0;
    }
    if (state->IsChildPossibleOrig(i) && !state->DoesExistChild(i)) {
	  KNState* childstate = state->getChild(i);
      state->SetExistingChild(i, childstate);
      MWBackUp(state, open, sus);
      if (state->AllChildrenInMemory()) {
	    open->pop(state);
	    state->_list = closedListIndex;		
      }
	  
      *used += 1;
      if (*used > MAX) {
	    KNState* last = processed->top();
		if (last != NULL) {
		  processed->pop();
		}
	    if (last == NULL && open->getLastWithNoChild()->GetFn() > res) {
    	  last = sus->popLastFromSus();
	    }
	    if (last == NULL) {
	      last = open->popLastWithNoChild();
     	}
    	//assert(last->GetDepth() > 0);  // Start node never gets deleted for this problem when there is at least NUM_V node memory.
    	DeleteState(last, used, open, sus, processed);
      }
      open->push(childstate);
      childstate->_list = openListIndex;
      return;
    }
  }
}

int MAStar_Window (KNState * state, int MAX, FILE* fp) {
  time_t start_time;
  time_t end_time;
  start_time = time(NULL);
  
  closedListIndex = -1;
  processedListIndex = 0;
  openListIndex = 1;
  susListIndex = 2;
  
  myPriorityQueue *open = new myPriorityQueue (0);
  myPriorityQueue *sus = new myPriorityQueue (0);
  myPriorityQueue *processed = new myPriorityQueue (2);

  int w = 0;
  int topLevel;
  int level;
  open->push (state);
  state->_list = openListIndex;
  int used = 1;
  int result = -1;
  while (1) {
    topLevel = -1;
    while (!open->empty ())	{
      KNState *s = (KNState *) open->top ();
	  level = s->GetDepth();
	  if (s->GetFn () <= result) {
        end_time = time(NULL);
        fprintf(fp, "%d %d %d\n", w, result, int(end_time - start_time));
		fflush(fp);
		break;
	  }
	  if (level > topLevel) {
        topLevel = level;
	  } else if (level <= topLevel - w) {
        open->pop();
	    mPushToSus(s, &used, open, sus, processed);
	    continue;
	  }
      end_time = time(NULL);
	  if (s->IsGoalState() || int(end_time - start_time) > MAX_T) {
		if (s->IsGoalState() && result < s->GetFn()) {
		  result = s->GetFn();
		  //s->print();
	    }
	    fprintf(fp, "%d %d %d\n", w, result, (int)(end_time - start_time));
		fflush(fp);
        break;
	  }
	  MAWGenerateChildren (s, open, sus, processed, &used, MAX, result);
	}
    if (int(end_time - start_time) > MAX_T) {
	  break;
	} 
    if (sus->empty()) {
	  break;
	} else {
      w++;
	  susListIndex++;
	  openListIndex = susListIndex - 1;
	  processedListIndex++;
	  processed->append(open);
	  open = sus;
	  sus = new myPriorityQueue(0);
    }
  }
  
  KNState* s;
  while(!processed->empty()) {
    s = (KNState *) processed->top ();
    processed->pop ();
    DeleteState(s, &used, open, sus, processed);
  }
  
  while (!sus->empty ()) {
    s = (KNState *) sus->top ();
    sus->pop ();
    DeleteState(s, &used, open, sus, processed);
  }

  while (!open->empty ()) {
    s = (KNState *) open->top ();
    open->pop ();
    DeleteState(s, &used, open, sus, processed);
  }
  
  delete open;
  delete sus;
  delete processed;
	  
  //assert(used == 0);
	  
  return result;
}
