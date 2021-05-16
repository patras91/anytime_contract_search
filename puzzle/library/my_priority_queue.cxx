// my_priority_queue_abstract.h implementation.
// Author: satya@cse.iitkgp.ernet.in

//#include "my_priority_queue.h"
//#include <stdio.h>


class myPriorityQueue {
 public:
myPriorityQueue(int flag) {
  s = NULL;
  next = this;
  prev = this;
  n = 0;
  _flag = flag;
}
  ~myPriorityQueue() {}

int df_state_comp(PuzState * a, PuzState * b) {
  //assert (a && b);
  if (a->GetDepth() < b->GetDepth()) {
    return 1;
  }
  if (a->GetDepth() > b->GetDepth()) {
    return 0;
  }
  if (a->GetFn() > b->GetFn()) {
    return 1;
  }
  return 0;
}

int my_state_comp(PuzState * a, PuzState * b) {
  switch(_flag) {/*
	case -1:  // ara*
	  return ara_state_comp(a, b);*/
	//case 0:  // sma*
	  //return state_comp(a, b); to be changed for sma*
	case 0:  // most followed
	  return state_comp(a, b);
	//case 0:  // simplest
	  //assert (a && b);
      //return a->GetFn () > b->GetFn ();
	case 1:  // beam-stack
	  //assert (a && b);
      return a->GetFn () >= b->GetFn ();
    case 2:  // stack
      return 0;
	case 3:  // dfbb
	  return df_state_comp(a, b);
	//case 4:  // g-value based
	  //return a->GetGn() > b->GetGn();
	default: // not allowed
	  assert (0);
  }
}

PuzState* top() {
  return this->next->s;
}

PuzState* bottom() {
  return this->prev->s;
}
  
short empty() {
  return n == 0;
}
  
void push(PuzState *s) {
  //assert(s != NULL);
  myPriorityQueue *t = this;
  while(t->next != this && my_state_comp(s, t->next->s)) {
    t = t->next;
  }
  myPriorityQueue *n = new myPriorityQueue(_flag);
  n->s = s;
  n->next = t->next;
  n->prev = t;
  n->next->prev = n;
  n->prev->next = n;
  this->n++;
}

void pop() {
  if (!empty()) {
    deleteLink(this->next);
  }
}

void deleteLink(myPriorityQueue* t) {
  t->prev->next = t->next;
  t->next->prev = t->prev;
  n--;
  delete t;
}

PuzState* popLast() {
  if (empty()) {
	return NULL;
  }	
  PuzState* s = this->prev->s;
  deleteLink(this->prev);
  return s;
}

myPriorityQueue* getLink(PuzState *s) {
  myPriorityQueue *t = this;
  while(t->next != this && s != t->next->s) {
    t = t->next;
  }
  if (t->next == this) {
	return NULL;
  }
  return t->next;
}

  int size() { return n; }
  myPriorityQueue* getNext() { return next; }
  myPriorityQueue* getPrev() { return prev; }
  PuzState* getState() { return s; }
  static bool state_comp(PuzState * a, PuzState * b) {
    //assert (a && b);
    if (a->GetFn () > b->GetFn ()) {
	  return 1;
	}
    if (a->GetFn () < b->GetFn ()) {
	  return 0;
	}
    return a->GetGn () < b->GetGn ();
  }

 //private:
  PuzState *s;
  myPriorityQueue *next;
  myPriorityQueue *prev;
  int n;
  int _flag;  // 0 for sma*, 1 for beam-stack, 2 for stack
};
