
class HHeap {
 public:
  HHeap(short comparison_flag) {
	_comparison_flag = comparison_flag;
	heap = new Heap*[nodes - 1];
	hhIndex = new int[nodes - 1];
	for (int i = 0; i < nodes - 1; i++) {
	  heap[i] = new Heap(comparison_flag);
	  hhIndex[i] = -1;
	}
	size = 0;
  }

  ~HHeap() {
	for (int i = 0; i < nodes - 1; i++) {
	  delete heap[i];
	}
	delete heap;
	delete hhIndex;
  }
  
  void deleteAllStates() {
	for (int i = 0; i < nodes - 1; i++) {
	  heap[i]->deleteAllStates();
	}
	while (hheap.size()) {
	  hheap.pop_back();
    }
	for (int i = 0; i < nodes - 1; i++) {
	  hhIndex[i] = -1;
	}
	size = 0;
  }
  
  void Heapify() {
	int idx = 0;
	for (int i = 0; i < nodes - 1; i++) {
	  heap[i]->Heapify();
	  if (!heap[i]->empty()) {
		hhIndex[i] = idx;
		hheap[idx] = heap[i]->top();
		idx++;
	  }
	}
	for (int i = parent(hheap.size() - 1); i >= 0; i--) {
	  heapify_down(i);
	}
  }

  void push(TspState* s) {
	int d = s->GetDepth();
	heap[d]->push(s);
	size++;
	if (hhIndex[d] != -1 && hheap[hhIndex[d]] == heap[d]->top()) {
	  return;
	}
	if (hhIndex[d] == -1) {
	  hheap.push_back(heap[d]->top());
	  hhIndex[d] = hheap.size() - 1;
      heapify_up(hhIndex[d]);
	} else {
      hheap[hhIndex[d]] = heap[d]->top();
      heapify_up(hhIndex[d]);
      heapify_down(hhIndex[d]);
    }
  }
  
  TspState* top() {
	return hheap.front();
  }

  void pop() {
    popIndex(top()->GetDepth(), 0);
  }

  void pop(TspState* s) {
    popIndex(s->GetDepth(), getExactIndex(s));
  }
  
  int getExactIndex(TspState* s) {
	return heap[s->GetDepth()]->getExactIndex(s);
  }
  
  TspState* getState(int depth, int index) {
	return heap[depth]->getState(index);
  }
  
  void popIndex(int depth, int index) {
	//TspState* s = heap[index];
	heap[depth]->popIndex(index);
	size--;
	if (!heap[depth]->empty() && heap[depth]->top() == hheap[hhIndex[depth]]) {
	  return;
	}
	int i = hhIndex[depth];
	if (heap[depth]->empty()) {
	  hhIndex[depth] = -1;
      if (i == hheap.size() - 1) {
        hheap.pop_back();
		return;
	  }
	  hheap[i] = hheap[hheap.size() - 1];
	  hhIndex[hheap[i]->GetDepth()] = i;
      hheap.pop_back();
    } else {
	  hheap[i] = heap[depth]->top();	  
    }
    heapify_up(i);
    heapify_down(i);
	//return s;
  }
/*
  void print() {
    vector<TspState*>::iterator pos = heap.begin();
    printf("HHeap = ");
    while ( pos != heap.end() ) {
      printf("%d ", ((TspState*)(*pos))->GetFn());
      ++pos;
    }
    printf("\n");
  }
  */
  bool empty() {
	return size == 0;
  }
  
  int Size() {
	return size;
  }

 private:
  
  void heapify_up(int index) {    
    //cout << "index=" << index << endl;
    //cout << "parent(index)=" << parent(index) << endl;
    //cout << "heap[parent(index)]=" << heap[parent(index)] << endl;
    //cout << "heap[index]=" << heap[index] << endl;
    int pindex = parent(index);
    while (index && pindex != -1 && Compare(hheap[pindex], hheap[index], _comparison_flag)) {
      TspState* tmp = hheap[pindex];
      hheap[pindex] = hheap[index];
      hheap[index] = tmp;
      hhIndex[hheap[index]->GetDepth()] = index;
      hhIndex[hheap[pindex]->GetDepth()] = pindex;
      index = pindex;
      pindex = parent(index);
    }
  }

  void heapify_down(int index) {     
    //cout << "index=" << index << endl;
    //cout << "left(index)=" << left(index) << endl;
    //cout << "right(index)=" << right(index) << endl;
    int lchild = left(index);
    bool l = lchild != -1 && Compare(hheap[index], hheap[lchild], _comparison_flag);
    int rchild = right(index);
    bool r = rchild != -1 && Compare(hheap[index], hheap[rchild], _comparison_flag);
    if (l && r) {
	  if (Compare(hheap[rchild], hheap[lchild], _comparison_flag)) {
		r = 0;
	  }	else {
		l = 0;
	  }
	}
	if (l) {
      TspState* tmp = hheap[index];
      hheap[index] = hheap[lchild];
      hheap[lchild] = tmp;
      hhIndex[hheap[index]->GetDepth()] = index;
      hhIndex[hheap[lchild]->GetDepth()] = lchild;
      heapify_down(lchild);
	}
	if (r) {
      TspState* tmp = hheap[index];
      hheap[index] = hheap[rchild];
      hheap[rchild] = tmp;
      hhIndex[hheap[index]->GetDepth()] = index;
      hhIndex[hheap[rchild]->GetDepth()] = rchild;
      heapify_down(rchild);
	}
  }

  int left(int parent) {
    int i = ( parent << 1 ) + 1; // 2 * parent + 1
    return ( i < hheap.size() ) ? i : -1;
  }

  int right(int parent) {
    int i = ( parent << 1 ) + 2; // 2 * parent + 2
    return ( i < hheap.size() ) ? i : -1;
  }

  int parent(int child) {
    if (child > 0) {
      return (child - 1) >> 1;
    }
    return -1;
  }
  
  int size;
  vector<TspState*> hheap;
  int* hhIndex;
  Heap** heap;
  short _comparison_flag;
};
