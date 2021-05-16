
class Heap {
 public:
  Heap(short comparison_flag) {
	_comparison_flag = comparison_flag;
  }

  ~Heap() {
  }
  
  void deleteAllStates() {
	while (heap.size()) {
	  delete heap.back();
	  heap.pop_back();
    }
    si.clear();
  }
  
  void Heapify() {
	for (int i = parent(size() - 1); i >= 0; i--) {
	  heapify_down(i);
	}
  }

  void push(PuzState* s) {
    heap.push_back(s);
    int n = heap.size() - 1;
    si[s] = n;
    heapify_up(n);
  }
  
  PuzState* top() {
	return heap.front();
  }

  void pop() {
    popIndex(0);
  }

  void pop(PuzState* s) {
	popIndex(getExactIndex(s));
  }
  
  int getExactIndex(PuzState* s) {/*
	for (int i = 0; i < size(); i++) {
	  if (s == heap[i]) {
		return i;
	  }
	}
	return -1;*/
	return si[s];
  }
  
  PuzState* getState(int index) {
	return heap[index];
  }
  
  void popIndex(int index) {
	//PuzState* s = heap[index];
    si.erase(heap[index]);
    PuzState* s = heap[heap.size() - 1];
	heap[index] = s;
	si[s] = index;
    heap.pop_back();
    heapify_up(index);
    heapify_down(index);
	//return s;
  }

  void print() {
    vector<PuzState*>::iterator pos = heap.begin();
    printf("Heap = ");
    while ( pos != heap.end() ) {
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
	return heap.size();
  }
  
  void heapify_up(int index) {    
    //cout << "index=" << index << endl;
    //cout << "parent(index)=" << parent(index) << endl;
    //cout << "heap[parent(index)]=" << heap[parent(index)] << endl;
    //cout << "heap[index]=" << heap[index] << endl;
    int pindex = parent(index);
    while (index && pindex != -1 && Compare(heap[pindex], heap[index], _comparison_flag)) {
      PuzState* tmp = heap[pindex];
      PuzState* s = heap[index];
      heap[pindex] = s;
      heap[index] = tmp;
      si[tmp] = index;
      si[s] = pindex;
      index = pindex;
      pindex = parent(index);
    }
  }

  void heapify_down(int index) {     
    //cout << "index=" << index << endl;
    //cout << "left(index)=" << left(index) << endl;
    //cout << "right(index)=" << right(index) << endl;
    int lchild = left(index);
    bool l = lchild != -1 && Compare(heap[index], heap[lchild], _comparison_flag);
    int rchild = right(index);
    bool r = rchild != -1 && Compare(heap[index], heap[rchild], _comparison_flag);
    if (l && r) {
	  if (Compare(heap[rchild], heap[lchild], _comparison_flag)) {
		r = 0;
	  }	else {
		l = 0;
	  }
	}
	if (l) {
      PuzState* tmp = heap[index];
      PuzState* s = heap[lchild];
      heap[index] = s;
      heap[lchild] = tmp;
      si[s] = index;
      si[tmp] = lchild;
      heapify_down(lchild);
	}
	if (r) {
      PuzState* tmp = heap[index];
      PuzState* s = heap[rchild];
      heap[index] = s;
      heap[rchild] = tmp;
      si[s] = index;
      si[tmp] = rchild;
      heapify_down(rchild);
	}
  }

  int left(int parent) {
    int i = ( parent << 1 ) + 1; // 2 * parent + 1
    return ( i < heap.size() ) ? i : -1;
  }

  int right(int parent) {
    int i = ( parent << 1 ) + 2; // 2 * parent + 2
    return ( i < heap.size() ) ? i : -1;
  }

  int parent(int child) {
    if (child > 0) {
      return (child - 1) >> 1;
    }
    return -1;
  }

  vector<PuzState*> heap;
  short _comparison_flag;
  map <PuzState*, int> si;  // state vs heap index
};
