// kn_state.h implementation.
// Author: satya@cse.iitkgp.ernet.in
//         sunandita.patra@cse.iitkgp.ernet.in

#include <new>
#include <stdio.h>
#include <stdlib.h>
//#include <assert.h>
#include <unistd.h>
#include <string.h>

#ifndef _KN_STATE_H_
#define _KN_STATE_H_

class KNState {
 public:
  KNState(int num_items);
  
  void SetExistingChild(int i, KNState *s);
  void ResetChildrenParent();
  int GetMaxChildFn();

  void SetFnOverEstimate();
  KNState* getChild(int i);
  short equals(KNState* s);
  KNState* Copy();

  short Completed();
  short AllChildrenInMemory();

  short IsChildPossibleOrig(int i);
  short IsGoalState();
  
  short IsChildPossible(int i);
  short ChildrenPossible();
  short GoalChildExist();
  
  int GetLastChildIndex() { return _last_child_index; }
  int GetTopIndex() { return top_index; }
  void SetLastChildIndex(int i) { _last_child_index = i; }
  void SetTopIndex(int i) { top_index = i; }

  int GetWeight() { return _weight; }
  void SetWeight(int w) { _weight = w; }
  
  void SetGn(int gn) { _gn = gn; }
  int GetGn() { return _gn; }
  
  void SetHn(float hn) { _hn = hn; }
  float GetHn() { return _hn; }
  
  float GetFn() { return this->_fn; }
  void SetFn(float fn) { _fn = fn; }
  void SetDepth(int depth) { _depth = depth; }
  int GetDepth() { return _depth; }

  void SetParent(KNState* parent) { _parent = parent; }
  KNState* GetParent() { return _parent; }
  void SetChosen(int i) { chosen[i] = 1;}
  bool isChosen(int i) { return chosen[i];}
  void print();
  int isSubSet(KNState *s);
  void copy(KNState *des, KNState *src);
	  
  ~KNState(){
    delete chosen;
  }
  int _list;
 private:
  int _gn;
  float _hn;
  
  float _fn;
  public: int _num_items;
	  
  //Following not to be copied.
  int _weight;
  int _depth;
  KNState* _parent;
  int _last_child_index;
  int top_index;
  int *chosen;
};

KNState::KNState(int num_items) {
  _num_items = num_items;
  _last_child_index = -1;
  top_index = -1;
  chosen = new int[num_items];
  for(int i = 0; i<num_items; i++) {
    chosen[i] = 0;
  }
}

void KNState::SetFnOverEstimate() {
  int t_w = GetWeight();
  float h = 0;
  bool isGoal = 1;
  //~ printf("\n top node = %d",top_index);
  for (int i = top_index + 1; i < _num_items; i++) {
	  if (GetWeight() + weight[sorted_index[i]] <= weight_limit) {
      isGoal = 0;
	  }
	  if (t_w < weight_limit) {    //t_w may be equal to weight_limit
	    t_w += weight[sorted_index[i]];
      if (t_w <= weight_limit) {
        h += value[sorted_index[i]];
      } else {
        h += ((weight_limit - (t_w - weight[sorted_index[i]])) * value[sorted_index[i]]*1.0 / weight[sorted_index[i]]);
        //~ break;                //don't break because goal will not be checked properly
      }
    }
    //~ printf("h = %f",h);
  }
  if (isGoal) {
    h = 0;
  }
  SetFn(GetGn() + h);
  SetHn(h);
}

KNState* KNState::getChild(int i) {
  KNState *child = new KNState(num_items);
  if(child == NULL) {
  	printf("\n Memory cannot be allocated.");
  	exit(1);
  }
  child->SetWeight(GetWeight() + weight[sorted_index[i]]);
  child->SetGn(GetGn() + value[sorted_index[i]]);
  child->SetParent(this);
  child->SetDepth(GetDepth() + 1);
  child->SetTopIndex(i);
  for(int j = 0; j < num_items; j++) {
    if(this->isChosen(j)) {
      child->SetChosen(j);
    }
  }
  child->SetChosen(i);
  child->SetFnOverEstimate();
  return child;
}

short KNState::IsChildPossibleOrig(int i) {
  if (GetWeight() + weight[sorted_index[i]] <= weight_limit) {
    return 1;
  }
  return 0;
}

short KNState::IsGoalState() {
  for (int i = top_index + 1; i < _num_items; i++) {
    if (IsChildPossibleOrig(i)) {
      return 0;
    }
  }
  return 1;
}
  
short KNState::IsChildPossible(int i) {
  if(top_index >= _num_items - 1) {
    return 0;
  } else {
    return IsChildPossibleOrig(i);
  }
}

void KNState::print() {
  printf("\n items:");
  for(int i = 0; i<_num_items; i++) {
    if(chosen[i]) {
      printf("\t%d",sorted_index[i]+1);
    }
  }
  printf("\ngn: %d fn: %f hn: %f weight = %d--------\n", GetGn(),GetFn(), GetHn(), GetWeight());
}

int KNState::isSubSet(KNState *s) {
  for(int i = 0; i<num_items; i++) {
    if(s->isChosen(i) && !isChosen(i)) {
      return 0;
    }
  }
  return 1;
}

void copy(KNState *des, KNState *src) {
  for(int i = 0; i<num_items; i++) {
    if(src->isChosen(i)) {
      des->SetChosen(i);
    }
  }
  des->SetFn(src->GetFn());
  des->SetGn(src->GetGn());
  des->SetHn(src->GetHn());
  des->SetWeight(src->GetWeight());
  des->SetTopIndex(src->GetTopIndex());
}


int resSubSet(KNState **list, KNState *s) {
  for(int i = 0; i<numK; i++) {
    if(list[i]->isSubSet(s)) {
      return 1;
    }
  }
  return 0;
}

#endif
