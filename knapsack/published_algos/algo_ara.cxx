
void ImprovePath(myPriorityQueue *q, int* res) {
  while(!q->empty()) {
	KNState* s = q->top();
	if (s->GetARAFn() <= *res) {
	  return;
	}
	if (s->IsGoalState()) {
	  *res = s->GetARAFn();
	  //s->print();
	  printf("ara: %d\n", *res);
	  return;
	}
	q->pop();
    for (int i = 0; i < num_items; i++) {
      if (s->IsChildPossible(i)) {
   	    KNState *childstate = s->getChild(i);
    	if (childstate->GetFn() > *res) {
          childstate->SetEpsilon(epsilon);
   	      q->push (childstate);
	    } else {
		  delete childstate;
		}
   	  }
    }
	delete s;	
  }
}

int ARA() {
  myPriorityQueue* q = new myPriorityQueue(-1);    
  epsilon = 0.5;
  KNState* s = CreateInitState();
  s->SetEpsilon(epsilon);
  q->push(s);
  int res = -1;
  ImprovePath(q, &res);
  
  while(epsilon < 1.0) {
	epsilon += 0.1;	
    myPriorityQueue* n_q = new myPriorityQueue(-1);
	while (!q->empty()) {
	  s = q->top();
	  q->pop();
      if (s->GetFn() > res) {
	    s->SetEpsilon(epsilon);
	    n_q->push(s);
	  } else {
		delete s;
	  }
	}
	delete q;
	q = n_q;
    ImprovePath(q, &res);
  }
    
  while (!q->empty()) {
    s = (KNState *) q->top ();
    q->pop ();
    delete s;
  }
  delete q;
  
  return res;
}
