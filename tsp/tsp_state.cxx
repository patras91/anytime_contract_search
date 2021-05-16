/**
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
*/

class TspState {
 public:
TspState (int depthp1) {
  path = new int[depthp1];
  _chosen = new bool[nodes];
  ana_key = -1;
}

void Initialize() {
  for (int i = 0; i < nodes; i++){
    _chosen[i] = 0;
  }
  _chosen[0] = 1;
  _topnode = 0;
  path[0] = 0;
  _gn = 0;  
  _depth = 0;

  _min_cov = 9999;  // this will help discard trivial min-cov 0.
  _max_cov = 0;
  _min_ucov = 1;
  _max_ucov = nodes - 1;
  sprintf(_hash, "%04d%04d%04d%04d%04d", _topnode, _min_cov, _max_cov, _min_ucov, _max_ucov);

  _hn = ComputeHn(0);
  _fn = _gn + _hn;
}

int getMinIdxAfterUpdatingDistances(int* distances, int* nodes_mst, int num_nodes_mst, int num_chosen, int* m) {
  int min_idx = -1;
  int min = MAX_RES;
  int last_city = nodes_mst[num_chosen - 1];
  for (int i = num_chosen; i < num_nodes_mst; i++) {
	if (distances[i] > GetDistance(last_city, nodes_mst[i])) {
	  distances[i] = GetDistance(last_city, nodes_mst[i]);
	}
	if (min > distances[i]) {
	  min = distances[i];
	  min_idx = i;
	}
  }
  *m = min;
  return min_idx;
}

int getMSTlengthPrim(int* nodes_mst, int num_nodes_mst) {
  int num_chosen = 1;
  
  int distances[num_nodes_mst];
  int min_idx = -1;
  int min = MAX_RES;
  for (int i = num_chosen; i < num_nodes_mst; i++) {
    distances[i] = GetDistance(nodes_mst[0], nodes_mst[i]);
	if (min > distances[i]) {
	  min = distances[i];
	  min_idx = i;
	}
  }
  
  int mst_length = 0;
  while (num_chosen != num_nodes_mst) {
    mst_length += min;
	int t = nodes_mst[min_idx];
	nodes_mst[min_idx] = nodes_mst[num_chosen];
	nodes_mst[num_chosen] = t;
	t = distances[min_idx];
	distances[min_idx] = distances[num_chosen];
	distances[num_chosen] = t;
    num_chosen++;
    min_idx = getMinIdxAfterUpdatingDistances(distances, nodes_mst, num_nodes_mst, num_chosen, &min);
  }
  return mst_length; 
}

int getMinEdgeLength(int nodeIdx, int* nodes_mst, int num_nodes_mst) {
  int min = MAX_RES;
  for (int j = 0; j < num_nodes_mst; j++) {
    int d = GetDistance(nodeIdx, nodes_mst[j]);
    if (d < min) {
	  min = d;
	}
  }
  return min;
}

int getMinEdgeLengthUpdateFrmMstOptNode(int* nodes_mst, int num_nodes_mst) {
  int min = MAX_RES;
  for (int j = 0; j < num_nodes_mst; j++) {
    int d = GetDistance(0, nodes_mst[j]);
    if (d < min) {
	  min = d;
	  _frm_mst_opt_node = nodes_mst[j];
	}
  }
  return min;
}

// not for atsps.
int ComputeHn(int frm_mst_opt_node) {
  int size = nodes - 1 - GetDepth();
  if (!size) {
	return GetDistance(GetTopNode(), 0);
  }
  int* nodes_mst = new int[size];
  int num_nodes_mst = 0;
  for (int i = 0; i < nodes; i++) {
    if (!IsChosen (i)) {
   	  nodes_mst[num_nodes_mst] = i;
   	  num_nodes_mst++;
   	}
  }
  //assert(size == num_nodes_mst);
  int mst_length = getMSTlengthPrim(nodes_mst, num_nodes_mst);
  int to_mst = getMinEdgeLength(GetTopNode(), nodes_mst, num_nodes_mst);
  int frm_mst;
  if (frm_mst_opt_node == GetTopNode()) {
    frm_mst = getMinEdgeLengthUpdateFrmMstOptNode(nodes_mst, num_nodes_mst);
  } else {
	frm_mst = GetDistance(0, frm_mst_opt_node);
	_frm_mst_opt_node = frm_mst_opt_node;
  }
  delete nodes_mst;
  return to_mst + mst_length + frm_mst;
}

TspState* GetNewChild(int topnode) {
  TspState *state = new TspState(_depth + 2);
  for (int i = 0; i < _depth + 1; i++) {
    state->path[i] = path[i];
    state->_chosen[i] = _chosen[i];
  }
  for (int i = _depth + 1; i < nodes; i++) {
    state->_chosen[i] = _chosen[i];
  }
  state->SetDepth(_depth + 1);
  state->path[state->GetDepth()] = topnode;
  state->_chosen[topnode] = 1;
  state->SetTopNode (topnode);
  
  state->SetGn(GetGn () + GetDistance(GetTopNode(), topnode));  
  state->SetHash(this);
  return state;
}

void UpdateFn(int hn, int frm_mst_opt_node, int pfn) {
  if (hn == -1) {
	SetHn(ComputeHn(frm_mst_opt_node));
  } else {
    SetHn(hn);
    _frm_mst_opt_node = frm_mst_opt_node;
  }
  int fn_u = GetGn() + GetHn();
  if (fn_u >= pfn) {
    SetFn(fn_u);
  } else {
    SetFn(pfn);
  }
}
  
  char* GetHash() {
	return _hash;
  }

bool equals(TspState* s) {
  //assert(GetDepth() == s->GetDepth() && GetTopNode() == s->GetTopNode());
  for (int i = 1; i < GetDepth(); i++) {
	if (!s->IsChosen(path[i])) {
	  return 0;
	}
  }
  return 1;
}

  int GetGn() { return _gn; }
  void SetGn(int gn) { _gn = gn; }
     
  int GetHn() { return this->_hn; }
  void SetHn(int hn) { _hn = hn; }
      
  int GetFn() { return this->_fn; }
  void SetFn(int fn) { _fn = fn; }
      
  bool IsChosen(int pos){ return _chosen[pos];}
  void SetTopNode(int pos){  _topnode=pos;}
  int GetTopNode(){ return _topnode;}
  void SetDepth(int depth) { _depth = depth; }
  int GetDepth() { return _depth; }
  
  short IsGoalState() { return _depth == nodes - 2; }
  int GetARAFn(double epsilon) { return ceil(_gn + epsilon * _hn); }
  
  double GetANAkey() {
	if (result_ana != ana_res || ana_key == -1) {
	  ana_res = result_ana;
	  if (GetHn()) {
	    ana_key = (ana_res - GetGn()) * 1.0 / GetHn();
      } else {
	    ana_key = MAX_RES - GetGn();
      }
	}
    return ana_key;
  }
  
  void print() {
    for (int i = 0; i < nodes; i++) {
	  if (_chosen[i]) {
		printf("%d ", i);
	  }
	}
	printf("\ngn: %d, hn: %d, fn: %d, hash: %s\n", _gn, _hn, _fn, _hash);
  }
	  
  ~TspState() {
	if (path) {
	  delete path;
    }
    if (_chosen) {
	  delete _chosen;
    }
  }

  double ana_key;
  int ana_res;
  int *path;
  
  int _min_cov, _max_cov, _min_ucov, _max_ucov;
  int _frm_mst_opt_node;
  
 private:
 
  int GetMinUCov() {
    for (int i = _min_ucov + 1; i < nodes; i++) {
	  if (!_chosen[i]) {
		return i;
	  }
	}
  }
 
  int GetMaxUCov() {
    for (int i = _max_ucov - 1; i >= 0; i--) {
	  if (!_chosen[i]) {
		return i;
	  }
	}
  }
 
void SetHash(TspState* parent) {
  _min_cov = parent->_min_cov;
  _max_cov = parent->_max_cov;
  _min_ucov = parent->_min_ucov;
  _max_ucov = parent->_max_ucov;
  if (_topnode < _min_cov) {
	_min_cov = _topnode;
  }  // else not to be used to deal with avoiding trivial min-cov 0 properly.
  if (_topnode > _max_cov) {
	_max_cov = _topnode;
  }
  if (_topnode == _min_ucov) {
	_min_ucov = GetMinUCov();
  } else if (_topnode == _max_ucov) {
	_max_ucov = GetMaxUCov();
  }
  sprintf(_hash, "%04d%04d%04d%04d%04d", _topnode, _min_cov, _max_cov, _min_ucov, _max_ucov);
  //assert(_hash[10] == '\0');
  //printf("-------%s\n", _hash);
}
  char _hash[21];
  int _gn;
  int _hn;
  int _fn;
  int _topnode;
  bool *_chosen;
	  
  //Following not to be copied.
  int _depth;
};
