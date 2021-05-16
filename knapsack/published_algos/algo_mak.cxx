

void DeleteStateK(KNState* s, int* used, myPriorityQueue* open, myPriorityQueue* sus, myPriorityQueue* processed) {
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
  if (s->ChildrenPossible() && s->GoalChildExist()) {
	  //printf("found!\n");
	  //storeList->push(s);
  delete s;
  s = NULL;
} else {
  if (s->GoalChildExist() && parent && !parent->goalChildDiscovered[s->GetTopItem()]) {
	  //assert(!s->HasChildren());
	  parent->goalChildDiscovered[s->GetTopItem()] = 1;
	  parent->_child_fn[s->GetTopItem()] = -1;
	  MWBackUp(parent, open, sus);
  }
  delete s;
  s = NULL;
}
  *used -= 1;
}

short MAKGenerateChildren (KNState *state, myPriorityQueue *open, myPriorityQueue *sus, myPriorityQueue* processed, int* used, int MAX, double res) {
  //assert(!state->AllChildrenInMemory());
  int start = -1;
  int node_id = state->GetTopItem ();
  int lastChildIndex = state->GetLastChildIndex();
  for (int i = lastChildIndex + 1; ; i++) {
    if (i == num_items) {
      i = 0;
    }
    if (i == start) {
	  return 0;
	}
	if (start == -1) {
	  start = i;
	}
    if (state->IsChildPossible(i) && (!state->DoesExistChild(i))) {
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
    	  last = sus->popLastWithNoChild();//FromSus();
	    }
	    if (last == NULL) {
	      last = open->popLastWithNoChild();
     	}
    	//assert(last->GetDepth() > 0);  // Start node never gets deleted for this problem when there is at least NUM_V node memory.
    	DeleteStateK(last, used, open, sus, processed);
      }
      open->push(childstate);
      childstate->_list = openListIndex;
      return 1;
    }
  }
}

void UpdateResultM(KNState* s, int* result, FILE* fp, int w, time_t start_time) {
  s = s->Copy();

  if (topksize_result == NULL) {
	current_top_K_size_sum = 0;
	topksize_result = new myPriorityQueue(0);
  }
  
  int prev_sum = current_top_K_size_sum;
  
  myPriorityQueue* t = topksize_result;
  for (int i = 0; i < topksize_result->size(); i++) {
	if (s->equals(t->next->s)) {
	  delete s;
	  return;
	}
	t = t->next;
  }

		if (topksize_result->size() < K) {
		  topksize_result->push(s);
		  current_top_K_size_sum += s->GetFn();//-1;
		} else {
		    //open->pop();
		    topksize_result->push(s);
			KNState* st = topksize_result->popLast();
			current_top_K_size_sum += s->GetFn() - st->GetFn();
			//DeleteState(st, &used, open, sus, processed);		    
			delete st;
		}
		if (topksize_result->size() == K) {
		  *result = topksize_result->bottom()->GetFn();//-1;
		}
		if (prev_sum != current_top_K_size_sum) {
          time_t end_time = time(NULL);
          fprintf(fp, "%d %f %d %d\n", w, current_top_K_size_sum, topksize_result->size(), (int)(end_time - start_time));
	      fflush(fp);
	      //topksize_result->print();			
		}
}

double MAK (KNState * state, int MAX, FILE* fp) {
  time_t start_time;
  time_t end_time;
  start_time = time(NULL);
  
  closedListIndex = -1;
  processedListIndex = 0;
  openListIndex = 1;
  susListIndex = 2;
  
  myPriorityQueue *open = new myPriorityQueue (0);
  myPriorityQueue *sus = new myPriorityQueue (0);
  //list<KNState*> closed;
  myPriorityQueue *processed = new myPriorityQueue (2);

  int w = 0;
  int topLevel;
  int level;
  open->push (state);
  state->_list = openListIndex;
  int used = 1;
  int result = -1;
  //short breakWithGoal;
  while (1)
    {
      topLevel = -1;
      while (!open->empty ())
	{
	  KNState *s = (KNState *) open->top ();
	  level = s->GetDepth();
	  if (s->GetFn () <= result)
	    {
		  break;
	    }
	  if (level > topLevel)
	    {
	      topLevel = level;
	    }
	  else if (level <= topLevel - w)
	    {
	      open->pop();
	      mPushToSus(s, &used, open, sus, processed);
	      continue;
	    }
          end_time = time(NULL);
	  if (s->IsGoalState () || ((int) (end_time - start_time)) > MAX_T)
	    {
			if (s->IsGoalState()) {

                  UpdateResultM(s, &result, fp, w, start_time);
	  }
	      break;
	    }
  //time_t start_time1 = time(NULL);
	  short suc = MAKGenerateChildren (s, open, sus, processed, &used, MAX, result);
	  if (!suc) {//printf("lala\n");
	  if (!s->HasChildren()) {
	  open->pop();
	  KNState* parent = s->GetParent();
	  if (parent) {
	  int uid = s->GetTopItem();
	  parent->goalChildDiscovered[uid] = 1;
      DeleteStateK(s, &used, open, sus, processed);	  //TODO: removed _completed and evaluate all cases of goal
	  parent->_child_fn[uid] = -1;
	  MWBackUp(parent, open, sus);
      } else {
      DeleteStateK(s, &used, open, sus, processed);	  //TODO: removed _completed and evaluate all cases of goal
      }
	  } else {
		  open->pop();
		  s->_list = closedListIndex;
	  //MWBackUp(s, open, sus);
	  }
      }
	//  printf("mastar child gentd: %d sec\n", (int)(time(NULL) - start_time1));
	}
     if (((int) (end_time - start_time)) > MAX_T) {
	   break;
	 }
	 if (sus->empty () && (open->empty() || open->top()->GetFn() <= result)) {
	  break;
	} else {
	  KNState* s = open->top();
		  if (s != NULL && s->IsGoalState()) {
	  open->pop();
	  KNState* parent = s->GetParent();
	  if (parent) {
	  int uid = s->GetTopItem();
	  parent->goalChildDiscovered[uid] = 1;
      DeleteStateK(s, &used, open, sus, processed);	  //TODO: removed _completed and evaluate all cases of goal
	  parent->_child_fn[uid] = -1;
	  MWBackUp(parent, open, sus);
      } else {
      DeleteStateK(s, &used, open, sus, processed);	  //TODO: removed _completed and evaluate all cases of goal
      }
  }
	  if (result != -1 && w < 10) {  // num_items + 1) {
      w++;}
      while (!open->empty() && open->top()->GetFn() > result) {
		KNState* s = open->top();
		open->pop();
		sus->push(s);
		s->_list = susListIndex;
	  }
	  susListIndex++;
	  openListIndex = susListIndex - 1;
	  processedListIndex++;
	  processed->append(open);
	  open = sus;
	  sus = new myPriorityQueue(0);/*
      } else {
	    while (!sus->empty ()) {
        KNState *s = (KNState *) sus->top ();
	    sus->pop ();
	    open->push(s);
	    }
      }*/
    }/*
      if (sus->empty ())
	{
	  break;
	} else {
	w++;
	susListIndex++;
	openListIndex = susListIndex - 1;
	processedListIndex++;
	processed->append(open);
	open = sus;
	sus = new myPriorityQueue(0);
  }*/
}
if (topksize_result) {
//topksize_result->print();
          end_time = time(NULL);
          fprintf(fp, "%d %f %d %d\n", w, current_top_K_size_sum, topksize_result->size(), (int)(end_time - start_time));
	      fflush(fp);
}
	  KNState* s;
	  while(!processed->empty()) {
        s = (KNState *) processed->top ();
        processed->pop ();
        DeleteStateK(s, &used, open, sus, processed);
	  }
	  
      while (!sus->empty ())
      {
      s = (KNState *) sus->top ();
      sus->pop ();
      DeleteStateK(s, &used, open, sus, processed);
      }
      delete sus;

      while (!open->empty ())
      {
      s = (KNState *) open->top ();
      open->pop ();
      DeleteStateK(s, &used, open, sus, processed);
      }
      delete open;
	  
	  //assert(used == 0);
	  
  return result;
}
