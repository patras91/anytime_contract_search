// For Memory bounded versions where we need to remove an element not necessarily the top one from the queue etc.
// Author: satya@cse.iitkgp.ernet.in

#ifndef _MY_PRIORITY_QUEUE_H_
#define _MY_PRIORITY_QUEUE_H_

#include "../kn/kn_state.cxx"
//#include <assert.h>

class myPriorityQueue {
 public:
  myPriorityQueue(int flag);
  ~myPriorityQueue() {}
  
  void append(myPriorityQueue* q);
  KNState* bottom();
  short empty();
  void push(KNState *s);
  void pop();  
  void pop(KNState *s);
  KNState* top();
  int size() { return n; }
  
  myPriorityQueue* getNext() { return next; }
  KNState* getState() { return s; }
  
  KNState* getLastWithNoChild();
  KNState* popLastWithNoChild();
  KNState* popLastFromSus();
  KNState* popLast();
  void pruneNodes(int res);
  int ara_state_comp(KNState * a, KNState * b);
  //int ana_state_comp(KNState * a, KNState * b);
  int df_state_comp(KNState * a, KNState * b);
  
void print() {
  myPriorityQueue *t = this;
  while(t->next != this) {
	t->next->s->print();
    t = t->next;
  }
}
  
  static int state_comp(KNState * a, KNState * b) {
    //assert (a && b);
    if (a->GetFn () < b->GetFn ()) {
      return 1;
    }
    if (a->GetFn () > b->GetFn ()) {
      return 0;
    }
    if (a->GetDepth() >= b->GetDepth()) {
      return 0;
	}
    return 1;
  }
  
 //private:
  KNState *s;
  myPriorityQueue *next;
  myPriorityQueue *prev;
  int n;
  int _flag;  // 0 for sma*, 1 for beam-stack, 2 for stack

  void deleteLink(myPriorityQueue* t);
  int my_state_comp(KNState * a, KNState * b);
};

#endif
