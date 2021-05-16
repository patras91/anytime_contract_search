// my_priority_queue.h implementation.
// Author: satya@cse.iitkgp.ernet.in

#include "my_priority_queue.h"
#include <stdio.h>

myPriorityQueue::myPriorityQueue(int flag) {
  s = NULL;
  next = this;
  prev = this;
  n = 0;
  _flag = flag;
}

/*int myPriorityQueue::ara_state_comp(KNState * a, KNState * b) {
  //assert (a && b);
  if (a->GetARAFn() < b->GetARAFn()) {
    return 1;
  }
  if (a->GetARAFn() > b->GetARAFn()) {
    return 0;
  }
  if (a->GetDepth() >= b->GetDepth()) {
    return 0;
  }
  return 1;
}*/


// may be in-correct for knapsack
/*int myPriorityQueue::ana_state_comp(KNState * a, KNState * b) {
  //assert (a && b);
  double a_key = (result_ana - a->GetGn()) / a->GetHn();
  double b_key = (result_ana - b->GetGn()) / b->GetHn();
  
  if (a_key < b_key) {
    return 1;
  }
  if (a_key > b_key) {
    return 0;
  }
  if (a->GetDepth() >= b->GetDepth()) {
    return 0;
  }
  return 1;
}*/

int myPriorityQueue::df_state_comp(KNState * a, KNState * b) {
  //assert (a && b);
  if (a->GetDepth() < b->GetDepth()) {
    return 1;
  }
  if (a->GetDepth() > b->GetDepth()) {
    return 0;
  }
  if (a->GetFn() < b->GetFn()) {
    return 1;
  }
  return 0;
}

int myPriorityQueue::my_state_comp(KNState * a, KNState * b) {
  switch(_flag) {
	//~ case -1:  // ara*
	  //~ return ara_state_comp(a, b);
	case 0:  // sma*
	  return state_comp(a, b);
	case 1:  // beam-stack
	  //assert (a && b);
      if (a->GetFn () <= b->GetFn ()) {
        return 1;
      }
      return 0;
    case 2:  // depth first
	  return df_state_comp(a, b);
	case 3:  // ED
      if (a->GetGn () < b->GetGn ()) {
        return 1;
      }
      if (a->GetGn() == b->GetGn()) {
		return a->GetDepth() < b->GetDepth();
	  }
      return 0;
	/*case 4:  // ana*
	  return ana_state_comp(a, b);*/
	default:  // stack
	  //assert (a && b);
	  return 0;
  }
}

KNState* myPriorityQueue::top() {
  return this->next->s;
}

KNState* myPriorityQueue::bottom() {
  return this->prev->s;
}
  
short myPriorityQueue::empty() {
  return n == 0;
}
  
void myPriorityQueue::push(KNState *s) {
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

void myPriorityQueue::append(myPriorityQueue* q) {
  if (q->empty()) {
	delete q;
	return;
  }
  this->n += q->size();
  q->prev->next = this;
  q->next->prev = this->prev;
  this->prev->next = q->next;
  this->prev = q->prev;  
  delete q;
}

void myPriorityQueue::pop() {
  if (!empty()) {
    deleteLink(this->next);
  }
}

void myPriorityQueue::pop(KNState *s) {
  myPriorityQueue *t = this;
  while(t->next != this && t->next->s != s) {
    t = t->next;
  }
  //assert(t->next != this);
  //if (t->next != this) {
  deleteLink(t->next);
  //}
}

void myPriorityQueue::deleteLink(myPriorityQueue* t) {
  t->prev->next = t->next;
  t->next->prev = t->prev;
  n--;
  delete t;
}

/*KNState* myPriorityQueue::getLastWithNoChild() {
  myPriorityQueue* t = this;
  while(t->prev != this && (t->prev->s->HasChildren() || t->prev->s->GetDepth() == 0)) {
    t = t->prev;
  }
  return t->prev->s;
}*/

/*KNState* myPriorityQueue::popLastWithNoChild() {
  if (empty()) {
    return NULL;
  }
  myPriorityQueue* t = this;
  while(t->prev != this && (t->prev->s->HasChildren() || t->prev->s->GetDepth() == 0)) {
    t = t->prev;
  }
  if (t->prev == this) {
    return NULL;
  }
  KNState *s = t->prev->s;
  deleteLink(t->prev);
  return s;
}*/

// Ensures tree structure of sus.
KNState* myPriorityQueue::popLastFromSus() {
  if (empty() || (this->prev->prev == this && this->prev->s->GetDepth() == 0)) {
    return NULL;
  }
  myPriorityQueue* t = this;
  while (t->prev->prev != this && (t->prev->prev->s->GetFn() == this->prev->s->GetFn() || t->prev->s->GetDepth() == 0)) {
    t = t->prev;
  }
  KNState *s = t->prev->s;
  deleteLink(t->prev);
  /*
  // Testing code begin.
  t = this;
  while(t->next != this) {
    KNState* s1 = t->next->s;
	  while(s1->GetParent() != NULL) {
	    s1 = s1->GetParent();
	  }
	  //assert(s1->GetDepth() == 0);
	  t = t->next;
  }
  // Testing code end.
  */
  return s;
}

KNState* myPriorityQueue::popLast() {
  if (empty()) {
	return NULL;
  }	
  KNState* s = this->prev->s;
  deleteLink(this->prev);
  return s;
}

void myPriorityQueue::pruneNodes(int res) {
  myPriorityQueue *temp = this;
  temp = temp->getNext();
  KNState* kt = temp->getState();
  while (kt != NULL) {
	myPriorityQueue *t = temp->getNext();
    if (kt->GetFn() <= res) {
	  deleteLink(temp);
	  delete kt;
    }
    temp = t;
    kt = temp->getState();
  }
}




