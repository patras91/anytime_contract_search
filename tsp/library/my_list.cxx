
class List {
 public:
  List() {
  }

  ~List() {
  }
  
  void deleteAllStates() {
	while (list.size()) {
	  delete list.back();
	  list.pop_back();
    }
  }
  
  void clear() {
	while (list.size()) {
	  list.pop_back();
    }
  }
  
  void push(TspState* s) {
    list.push_back(s);
  }
  
  void pop(TspState* s) {
	popIndex(getExactIndex(s));
  }
  
  int getExactIndex(TspState* s) {
	for (int i = 0; i < size(); i++) {
	  if (s == list[i]) {
		return i;
	  }
	}
	return -1;
  }
  
  int getIndex(TspState* s) {
	for (int i = 0; i < size(); i++) {
	  if (s->equals(list[i])) {
		return i;
	  }
	}
	return -1;
  }
  
  TspState* getState(int index) {
	return list[index];
  }
  
  // dangerous. only to be used by hashtables.
  void updateIndex(int index, TspState* s) {
	list[index] = s;
  }
  
  void popIndex(int index) {
	//TspState* s = list[index];
	list[index] = list[list.size() - 1];
    list.pop_back();
	//return s;
  }

  void print() {
    vector<TspState*>::iterator pos = list.begin();
    printf("List = ");
    while ( pos != list.end() ) {
      printf("%d ", ((TspState*)(*pos))->GetFn());
      ++pos;
    }
    printf("\n");
  }
  
  bool empty() {
	return size() == 0;
  }

 private:
  int size() {
	return list.size();
  }
  
  vector<TspState*> list;
};
