
class beam_stack_item {
 public:
  beam_stack_item(int min, int max, int dp1) {
	f_min = min;
	f_max = max;
	this->dp1 = dp1;
	min_path = new int[dp1];
	max_path = new int[dp1];
	for (int i = 0; i < dp1; i++) {
	  min_path[i] = -1;
	  max_path[i] = -1;
	}
  }
  /*
  short MinOrAfterMinPath(int* _path) {
	for (int i = 0; i < dp1; i++) {
	  if (min_path[i] > _path[i]) {
		return 0;
	  } else if (min_path[i] < _path[i]) {
		return 1;
	  }
	}
	return 1;
  }
  */
  short EqualsMinPath(int* _path) {
	for (int i = 0; i < dp1; i++) {
	  if (min_path[i] != _path[i]) {
		return 0;
	  }
	}
	return 1;
  }
  
  void SetMinPath(int* _path) {
	for (int i = 0; i < dp1; i++) {
	  min_path[i] = _path[i];
	}	  
  }
  /*
  short EqualsMaxPath(int* _path) {
	for (int i = 0; i < dp1; i++) {
	  if (max_path[i] != _path[i]) {
		return 0;
	  }
	}
	return 1;
  }
  */
  void SetMaxPath(int* _path) {
	for (int i = 0; i < dp1; i++) {
	  max_path[i] = _path[i];
	}	  
  }
  
  void ResetMaxPath() {
	for (int i = 0; i < dp1; i++) {
	  max_path[i] = -1;
	}	  
  }
  
  int* GetMaxPath() { return max_path; }
  
  ~beam_stack_item() {
	delete min_path;
	delete max_path;
  }
  
  int f_min;
  int f_max;
  // For tie-breaking.
  int* min_path;
  int* max_path;
  int dp1;
};
