
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
  
  void push(PuzState* s) {
    list.push_back(s);
  }
  
  void pop(PuzState* s) {
	popIndex(getExactIndex(s));
  }
  
  int getExactIndex(PuzState* s) {
	for (int i = 0; i < size(); i++) {
	  if (s == list[i]) {
		return i;
	  }
	}
	return -1;
  }
  
  int getIndex(PuzState* s) {
	for (int i = 0; i < size(); i++) {
	  if (s->equals(list[i])) {
		return i;
	  }
	}
	return -1;
  }
  
  PuzState* getState(int index) {
	return list[index];
  }
  
  // dangerous. only to be used by hashtables.
  void updateIndex(int index, PuzState* s) {
	list[index] = s;
  }
  
  void popIndex(int index) {
	//PuzState* s = list[index];
	list[index] = list[list.size() - 1];
    list.pop_back();
	//return s;
  }

  void print() {
    vector<PuzState*>::iterator pos = list.begin();
    printf("List = ");
    while ( pos != list.end() ) {
      printf("%d ", ((PuzState*)(*pos))->GetFn());
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
  
  vector<PuzState*> list;
};
