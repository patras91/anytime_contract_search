// KN State.
// Author: satya@cse.iitkgp.ernet.in
//         sunandita.patra@cse.iitkgp.ernet.in
#ifndef _KN_STATE_H_
#define _KN_STATE_H_

#include <string.h>

//~ int openListIndex;
//~ int closedListIndex;
//~ int susListIndex;
//~ int processedListIndex;

int num_items;
int* value;
int* weight;
int weight_limit;

// For fn overestimate computation, max val/wt first
int*  sorted_value;
int*  sorted_weight;
int* sorted_index;

//~ float epsilon;

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
  //short Completed();
  //short AllChildrenInMemory();
  
  int GetLastChildIndex() { return _last_child_index; }
  void SetLastChildIndex(int i) { _last_child_index = i; }

  void ResetExistingChild(int i);
  //~ int GetExistingChildFn(int i) { return _child_fn[i]; }
  
  //~ int GetChildFn(int i) { return _child_fn[i]; }
  //~ void SetChildFn(int i, int fn) { _child_fn[i] = fn; }

  //~ short DoesExistChild(int i) { return _existing_children[i] != NULL; }
  short HasChildren();
  //short IsRootTraceable();

  int GetWeight() { return _weight; }
  void SetWeight(int w) { _weight = w; }
  
  void SetGn(int gn) { _gn = gn; }
  int GetGn() { return _gn; }
  
  void SetHn(float hn) { _hn = hn; }
  float GetHn() { return _hn; }
  
  float GetFn() { return this->_fn; }
  void SetFn(float fn) { _fn = fn; }
  //void SetTopItem(int pos) {  _top_item = pos; }
  //int GetTopItem() { return _top_item; }
  void SetDepth(int depth) { _depth = depth; }
  int GetDepth() { return _depth; }
  //~ KNState* GetExistingChild(int i) { return _existing_children[i]; }

  void SetParent(KNState* parent) { _parent = parent; }
  KNState* GetParent() { return _parent; }
  void print();
  
  //~ void SetEpsilon(float e) { epsilon = e; }
  //~ int GetARAFn() { return GetGn() + epsilon * GetHn(); }
  
  //int getMin();

  //int getMinNC();
  //KNState* Copy();
	  
	  
  ~KNState(){
	//~ delete path;
    //~ delete _child_fn;
    //~ delete _existing_children;

  }
  //~ int *path;
  //~ int* _child_fn;
  //int o_f;
  int _list;
 private:
  //Node* _node_list[MAX_NODES];
  int _gn;
  float _hn;
  //~ float epsilon;
  
  float _fn;
  //int _top_item;
  public: int _num_items;
	  
  //Following not to be copied.
  int _weight;
  int _depth;
  KNState* _parent;
  int _last_child_index;
  //~ KNState** _existing_children;
};

#endif
