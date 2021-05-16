// Author: satya@cse.iitkgp.ernet.in

class PuzState {
public:
PuzState(int depthp1) {
  //_last_child_index = -1;
  //_list = -1;
  ana_key = -1;  
  _puzzle = new int*[_size];
  for (int i = 0; i < _size; i++){
    _puzzle[i] = new int[_size];
  }
  /*
  for (int i = 0; i < 4; i++) {
    _child_fn[i] = -1;
    _existing_children[i] = NULL;
  }
  */
  path = new int[depthp1];
  /*
  for (int i = 0; i < depthp1; i++) {
	path[i] = -1;
  }*/
}

void SetPuzzle(int **puzzle) {
  for (int i = 0; i < _size; i++) {
	for (int j = 0; j < _size; j++) {
	  _puzzle[i][j] = puzzle[i][j];
	  if (_puzzle[i][j] == -1) {
		_row_index = i;
		_col_index = j;
	  }
	}
  }
}

void printPuzzle() {
  //printf("_hash: %s\n\n", _hash);
  for(int i = 0; i < _size; i++) {
    for (int j = 0; j < _size; j++) {
        printf("%d\t", _puzzle[i][j]);
    }
    printf("\n");
  }  
  printf("gn: %d, hn: %d, fn: %d\n", _depth, _hn, _fn);
  printf("_hash: %s\n\n", _hash);
}

void move(int dir) {
  SetDir(dir);
  switch (dir) {
    case 0:  // move left
      _puzzle[_row_index][_col_index] = _puzzle[_row_index][_col_index - 1];
      _col_index -= 1;
      break;
    case 1:  // move up
      _puzzle[_row_index][_col_index] = _puzzle[_row_index - 1][_col_index];
      _row_index -= 1;
      break;
    case 2:  // move right
      _puzzle[_row_index][_col_index] = _puzzle[_row_index][_col_index + 1];
      _col_index += 1;
      break;
    case 3:  // move down
      _puzzle[_row_index][_col_index] = _puzzle[_row_index + 1][_col_index];
      _row_index += 1;
      break;
  }
  _puzzle[_row_index][_col_index] = -1;
}

void Initialize(int **puzzle) {  
  SetPuzzle(puzzle);
  SetHKey();
  _dir = -1;
  SetDepth(0);
  path[0] = _dir;
  
  int hn = 0;
  for (int i = 0; i < _size; i++) {
	for (int j = 0; j < _size; j++) {
	  if (_puzzle[i][j] != -1) {
		int orig_row_index = _puzzle[i][j] / _size;
		int orig_col_index = _puzzle[i][j] % _size;
		hn += fabs(i - orig_row_index) + fabs(j - orig_col_index);
	  }
	}
  }
  //printf("hn: %d\n", hn);
  SetHn(hn);
  SetFn(GetGn() + hn);
  SetHash();
  //SetParent(NULL);
}

short equals(PuzState* s) {
  // Assuming size will be equal.
/*  if (strcmp(_hash, s->GetHash()) != 0) {
	return 0;
  }*/
  //assert(strcmp(_hash, s->GetHash()) == 0);
  /*if (GetHn() != s->GetHn()) {  // Small performance improvement.
	return 0;
  }*/
  //assert(_row_index == s->_row_index && _col_index == s->_col_index);
  for(int i = 0; i < _size; i++) {
    for (int j = 0; j < _size; j++) {
      if (_puzzle[i][j] != s->_puzzle[i][j]) {
		return 0;
	  }
    }
  }
  return 1;
}

int getManhattanHn(int phn) {/*
  int hn = 0;
  for (int i = 0; i < _size; i++) {
	for (int j = 0; j < _size; j++) {
	  if (_puzzle[i][j] != -1) {
		int orig_row_index = _puzzle[i][j] / _size;
		int orig_col_index = _puzzle[i][j] % _size;
		hn += fabs(i - orig_row_index) + fabs(j - orig_col_index);
	  }
	}
  }
  //printf("hn: %d\n", hn);
  return hn;*/
  int orig_col_index, orig_row_index;
  switch (_dir) {
    case 0:  // move left
      orig_col_index = _puzzle[_row_index][_col_index + 1] % _size;
      return phn + fabs(_col_index + 1 - orig_col_index) - fabs(_col_index - orig_col_index);
    case 1:  // move up
      orig_row_index = _puzzle[_row_index + 1][_col_index] / _size;
      return phn + fabs(_row_index + 1 - orig_row_index) - fabs(_row_index - orig_row_index);
    case 2:  // move right
      orig_col_index = _puzzle[_row_index][_col_index - 1] % _size;
      return phn + fabs(_col_index - 1 - orig_col_index) - fabs(_col_index - orig_col_index);
    case 3:  // move down
      orig_row_index = _puzzle[_row_index - 1][_col_index] / _size;
      return phn + fabs(_row_index - 1 - orig_row_index) - fabs(_row_index - orig_row_index);
  }
}

void ComputeFn(int phn) {
  SetHn(getManhattanHn(phn));
  SetFn(GetGn() + GetHn());
}

void SetHKey() {
  hkey = _row_index * _size + _col_index;
}

PuzState* getChild(int dir, int phn) {
  //assert(IsChildPossible(dir));
  PuzState *state = new PuzState(_depth + 2);
  state->SetPuzzle(_puzzle);
  state->move(dir);
  state->SetHKey();
  state->SetDepth(_depth + 1);
  state->ComputeFn(phn);/*
  if (state->GetFn() < GetChildFn(dir)) {  // only happens when there is deletion.
    /*
	printf("Childstate:\n");
	state->printPuzzle();
	printf("State:\n");
	printPuzzle();
	for (int i = 0; i < 4; i++) {
	  printf("_child_fn[%d] = %d\n", i, _child_fn[i]);
	  if (DoesExistChild(i)) {
		printf("existing fn: %d\n", _existing_children[i]->GetFn());
	  }
	}
	//assert(0);
	*
	state->SetFn(GetChildFn(dir));
  }*/
  if (state->GetFn() < GetFn()) {
	state->SetFn(GetFn());
  }
  //state->SetParent(this);
  
  for (int i = 0; i <= GetDepth(); i++) {
	state->path[i] = path[i];
  }
  state->path[GetDepth() + 1] = dir;
  state->SetHash();
  
  return state;
}

bool IsGoalState() {/*
  for(int i = 0; i < _size; i++) {
    for (int j = 0; j < _size; j++) {
      if (_puzzle[i][j] != i * _size + j) {
		if (i == 0 && j == 0) {
		  if (_puzzle[i][j] != -1) {
		    return 0;
	      }
	    } else {
		  return 0;
		}
	  }
    }
  }
  return 1;*/
  return _hn == 1;  
}
/*
void ResetExistingChild(int i) {
  if (_existing_children[i] == NULL) {
	printPuzzle();
	//assert(0);
  }
  //assert(_child_fn[i] <= _existing_children[i]->GetFn());
  _child_fn[i] = _existing_children[i]->GetFn();
  _existing_children[i] = NULL;
}

void SetExistingChild(int i, PuzState *s) {
  //assert(!DoesExistChild(i));
  _last_child_index = i;
  _existing_children[i] = s;
}

void ResetChildrenParent() {
  for (int i = 0; i < 4; i++) {
    if (_existing_children[i] != NULL) {
      _existing_children[i]->SetParent(NULL);
    }
  }
}

int GetMinChildFn() {
  int ans = 9999999;
  for (int i = 0; i < 4; i++) {
    int child_fn;
    if (DoesExistChild(i)) {
      child_fn = _existing_children[i]->GetFn();
    } else {
      child_fn = _child_fn[i];
    }
    //assert(child_fn < 9999999);
    if (child_fn < ans && child_fn != -1) {
      ans = child_fn;
    }
  }
  if (ans == 9999999) {
    return GetFn();
  }
  return ans;
}

short Completed() {
  for (int i = 0; i < 4; i++) {
    if (IsChildPossible(i) && !DoesExistChild(i) && _child_fn[i] == -1) {
      return 0;
    }
  }
  return 1;
}
*/
short IsChildPossible(int i) {/*
  if (IsChildNotUseful(i)) {
	return 0;
  }*/
  switch (_dir) {
	case 0:
	  if (i == 2) {
		return 0;
	  }
	  break;
	case 1:
	  if (i == 3) {
		return 0;
	  }
	  break;
	case 2:
	  if (i == 0) {
		return 0;
	  }
	  break;
	case 3:
	  if (i == 1) {
		return 0;
	  }
	  break;
  }
  switch (i) {
	case 0:
	  if (_col_index > 0) {
		return 1;
	  }
	  break;
	case 1:
	  if (_row_index > 0) {
		return 1;
	  }
	  break;
	case 2:
	  if (_col_index < _size - 1) {
		return 1;
	  }
	  break;
	case 3:
	  if (_row_index < _size - 1) {
		return 1;
	  }
	  break;
  }
  return 0;
}
/*
short AllChildrenInMemory() {
  for (int i = 0; i < 4; i++) {
    if (IsChildPossible(i) && !DoesExistChild(i)) {
      return 0;
    }
  }
  return 1;
}

short HasChildren() {
  for (int i = 0; i < 4; i++) {
    if (DoesExistChild(i)) {
	  return 1;
	}
  }
  return 0;
}
  
  int GetLastChildIndex() { return _last_child_index; }*/
/*
  int GetChildFn(int i) { return _child_fn[i]; }
  void SetChildFn(int i, int fn) { _child_fn[i] = fn; }

  short DoesExistChild(int i) { return _existing_children[i] != NULL; }
  */
  int GetGn() { return _depth; }
  
  int GetHn() { return this->_hn; } 
  
  int GetFn() { return this->_fn; }      
  void SetFn(int fn) { _fn = fn; }
  
  int GetDir() { return this->_dir; }      
  void SetDir(int dir) { _dir = dir; }
      
  void SetDepth(int depth) { _depth = depth; }
  int GetDepth() { return _depth; }
/*
  void SetChildNotUseful(int i) { _child_fn[i] = 9999998; }
  int IsChildNotUseful(int i) {	return _child_fn[i] == 9999998; }
	  
  PuzState* GetChild(int i) { return _existing_children[i]; }
  
  void SetParent(PuzState* parent) { _parent = parent; }
  PuzState* GetParent() { return _parent; }
  
  void SetEpsilon(float e) { epsilon = e; }
  int GetARAFn() { return _gn + epsilon * _hn; }
      */
  
  double GetANAkey() {
	if (result_ana != ana_res || ana_key == -1) {
	  ana_res = result_ana;
	  if (GetHn()) {
	    ana_key = (ana_res - GetGn()) * 1.0 / GetHn();
      } else {
	    ana_key = MAX_D - GetGn();
      }
	}
    return ana_key;
  }
  
  char* GetHash() {
	return _hash;
  }
  
  int GetHKey() {
	return hkey;
  }
	  
  ~PuzState() {
    delete path;
	for (int i = 0; i < _size; i++) {
	  delete _puzzle[i];
	}
	delete _puzzle;
  }
      
  //int _list;

  int **_puzzle;
  int *path;
  
 private:     
  void SetHn(int hn) { _hn = hn; }

void SetHash() {
  int left = _col_index == 0 ? 0 : _puzzle[_row_index][_col_index - 1];
  int top = _row_index == 0 ? 0 : _puzzle[_row_index - 1][_col_index];
  int right = _col_index == _size - 1 ? 0 : _puzzle[_row_index][_col_index + 1];
  int bottom = _row_index == _size - 1 ? 0 : _puzzle[_row_index + 1][_col_index];
  sprintf(_hash, "%02d%02d%02d%02d%03d", left, top, right, bottom, _hn);
  //sprintf(_hash, "%d%d%02d%02d%02d%02d%03d", _row_index, _col_index, left, top, right, bottom, _hn);
  //assert(_hash[10] == '\0');
  //printf("-------%s\n", _hash);
}

  /*
  _hash[0] = _row_index + '0';
  _hash[1] = _col_index + '0';
  //sprintf(_hash, "%d%d", _row_index, _col_index);
  if (_col_index == 0) {
	_hash[2] = '0';
	_hash[3] = '0';
  } else {
	sprintf(&_hash[2], "%02d", _puzzle[_row_index][_col_index - 1]);/*
	int t = _puzzle[_row_index][_col_index - 1];
	hash[2] = '0' + t / 10;
	hash[3] = '0' + t % 10;*
  }
  if (_row_index == 0) {
	_hash[4] = '0';
	_hash[5] = '0';
  } else {
	sprintf(&_hash[4], "%02d", _puzzle[_row_index - 1][_col_index]);
  }
  if (_col_index == _size - 1) {
	_hash[6] = '0';
	_hash[7] = '0';
  } else {
	sprintf(&_hash[6], "%02d", _puzzle[_row_index][_col_index + 1]);
  }
  if (_row_index == _size - 1) {
	_hash[8] = '0';
	_hash[9] = '0';
  } else {
	sprintf(&_hash[8], "%02d", _puzzle[_row_index + 1][_col_index]);
  }
  _hash[10] = '\0';*/
  
  char _hash[12];
  double ana_key;
  int ana_res;
  
  int _row_index, _col_index;
  int _hn;
  //float epsilon;
  int _fn;
  int _depth;
  int hkey;
  //PuzState* _parent;
  //int _last_child_index;
  //PuzState* _existing_children[4];
  //int _child_fn[4];
  int _dir;
};
