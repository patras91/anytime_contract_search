
void PruneLayer(myPriorityQueue* q, int w) {
  if (q->size() <= w) {
    return;
  }
  KNState* s;
  while (q->size() > w) {
	s = q->popLast();
	if (q->size() > w) {
	  delete s;
    }
  }
  beam_stack_item* t = beam_stack.top();
  t->f_max = s->GetFn();
  t->SetMaxPath(s->path);
  delete s;
}

void GenerateChildrenBS(KNState* state, myPriorityQueue* q, short* minFound) {
  //assert(beam_stack.size() == state->GetDepth() + 2);
  beam_stack_item* t = beam_stack.top();
  //assert(t->f_min >= t->f_max);
  /* The commented code has bugs (related to minFound). Fix before usage.
   * Role can be replaced by simple but slightly more temp. memory consuming version below.
  short minFound = 0, maxFound = 0;
  if (t->min_path[0] == -1) {
	minFound = 1;
  }
  if (t->max_path[0] == -1) {
	maxFound = 1;
  }
  for (int i = 0; i < num_items; i++)
    {
      if (IsChildPossible(state, i))
    	{
    	  KNState *childstate = getChild(state, i);
		  if (minFound) {
			if (maxFound) {
			  if (childstate->GetFn() <= t->f_min && childstate->GetFn() > t->f_max) {
		        q->push(childstate);
			  } else {
			    delete childstate;
			  }
			} else {
			  if (childstate->GetFn() <= t->f_min && (childstate->GetFn() > t->f_max || (childstate->GetFn() == t->f_max && t->max_path[0] != -1 && !t->EqualsMaxPath(childstate->path)))) {
			    q->push(childstate);
			  } else {
				if (t->EqualsMaxPath(childstate->path)) {
				  maxFound = 1;
				}
			    delete childstate;
			  }
		    }
		  } else {
		    if (((childstate->GetFn() == t->f_min && (t->min_path[0] == -1 || t->EqualsMinPath(childstate->path))) || childstate->GetFn() < t->f_min) && (childstate->GetFn() > t->f_max || (childstate->GetFn() == t->f_max && t->max_path[0] != -1 && !t->EqualsMaxPath(childstate->path)))) {
    	      q->push (childstate);
			  if (t->EqualsMinPath(childstate->path)) {
			    minFound = 1;
		      }
			  //printf("\n");
	        } else {
		      //printf("deleted min not found.\n");
			  delete childstate;
		    }
		  }
    	}
    }
    */
  for (int i = 0; i < num_items; i++) {
    if (state->IsChildPossible(i)) {
   	  KNState *childstate = state->getChild(i);
      state->SetChildFn(i, childstate->GetFn());
	  if (childstate->GetFn() <= t->f_min && childstate->GetFn() >= t->f_max) {
        q->push(childstate);
	  } else {
	    delete childstate;
	  }
    }
  }
  
  if (!(*minFound) && t->min_path[0] != -1) {
    myPriorityQueue* temp = q;
    temp = temp->getNext();
    KNState* kt = temp->getState();
    while (kt != NULL && !t->EqualsMinPath(kt->path)) {
	  temp = temp->getNext();
	  if (kt->GetFn() == t->f_min) {
	    q->pop(kt);
	    delete kt;
      }
      kt = temp->getState();
    }
    if (kt != NULL && t->EqualsMinPath(kt->path)) {
	  *minFound = 1;
	}
  }
}
  
int search(myPriorityQueue* open[], int l, int U, int w, time_t start_time, FILE* fp) {
  //assert(beam_stack.size() == l + 1);
  for (int i = l; i <= num_items; i++) {
    while (!open[i]->empty()) {
      KNState* s = open[i]->popLast();
      delete s;
    }
  }
  l = l - 1;
  
  while (l < num_items + 1 && !open[l]->empty()) {
	myPriorityQueue* temp = open[l];
    temp = temp->getNext();
    short minFound = 0;
	while (temp->getState() != NULL) {
	  KNState* node = temp->getState();
	  if (node->IsGoalState()) {/*
		if (U < node->GetFn()) {
		  U = node->GetFn();
		  //node->print();
	    }*/
	    UpdateResultM(node, &U, fp, w, start_time);
	  } else {
	    GenerateChildrenBS(node, open[l + 1], &minFound);
      }
	  
	  if (int(time(NULL) - start_time) > MAX_T) {
		return U;  
	  }
	  temp = temp->getNext();
	}
	if (l + 1 <= num_items) {
	  PruneLayer(open[l + 1], w);
    }
	l = l + 1;
	beam_stack.push(new beam_stack_item(9999999, U, num_items + 1));
  }
  return U;
}

int Beam_Stack(int MAX, FILE* fp) {
  time_t start_time;
  time_t end_time;
  start_time = time(NULL);
  
  openListIndex = 0;
  myPriorityQueue* open[num_items + 1];
  for (int i = 0; i <= num_items; i++) {
	open[i] = new myPriorityQueue(1);
  }
  open[0]->push(CreateInitState());
  int U = -1;
  beam_stack.push(new beam_stack_item(9999999, U, num_items + 1));
  beam_stack.push(new beam_stack_item(9999999, U, num_items + 1));

  while (beam_stack.size() > 1) {
	U = search(open, beam_stack.size() - 1, U, MAX / (num_items + 1), start_time, fp);
    end_time = time(NULL);
	if (end_time - start_time > MAX_T) {
	  while(beam_stack.size() > 1) {
	    beam_stack.pop();
	  }
	}/* else if (U < t) {
	  U = t;
	  fprintf(fp, "%d %d\n", U, (int)(end_time - start_time));
      fflush(fp);
	}*/
	while (beam_stack.size() > 1 && (beam_stack.top())->f_max <= U) {
	  beam_stack_item* t = beam_stack.top();
	  beam_stack.pop();
	  delete t;
	}
	if (beam_stack.size() <= 1) {/*
      end_time = time(NULL);
	  fprintf(fp, "%d %d\n", U, (int)(end_time - start_time));
      fflush(fp);*/
	  break;
	}
	beam_stack.top()->f_min = beam_stack.top()->f_max;
	beam_stack.top()->SetMinPath(beam_stack.top()->GetMaxPath());
	beam_stack.top()->f_max = U;
	beam_stack.top()->ResetMaxPath();
  }
          end_time = time(NULL);
          fprintf(fp, "%d %f %d %d\n", MAX / (num_items + 1), current_top_K_size_sum, topksize_result->size(), (int)(end_time - start_time));
	      fflush(fp);
  
  beam_stack_item* t = beam_stack.top();
  beam_stack.pop();
  delete t;
  
  for (int i = 0; i <= num_items; i++) {
    while (open[i] != NULL && !open[i]->empty()) {
	  KNState* s = open[i]->popLast();
      delete s;
    }
	if (open[i] != NULL) {
	  delete open[i];  
    }
  }
  return U;
}*/
