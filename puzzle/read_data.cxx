
int _row_index, _col_index;
int **_puzzle;

void move(int dir) {
  switch (dir) {
    case 0:  // left
      if (_col_index - 1 >= 0) {
        _puzzle[_row_index][_col_index] = _puzzle[_row_index][_col_index - 1];
        _col_index -= 1;
      }
      break;
    case 1:  // up
      if (_row_index - 1 >= 0) {
        _puzzle[_row_index][_col_index] = _puzzle[_row_index - 1][_col_index];
        _row_index -= 1;
      }
      break;
    case 2:  // right
      if (_col_index + 1 < _size) {
        _puzzle[_row_index][_col_index] = _puzzle[_row_index][_col_index + 1];
        _col_index += 1;
      }
      break;
    case 3:  // down
      if (_row_index + 1 < _size) {
        _puzzle[_row_index][_col_index] = _puzzle[_row_index + 1][_col_index];
        _row_index += 1;
      }
      break;
  }
  _puzzle[_row_index][_col_index] = -1;
}

void Create(int size, FILE* fp) {
  _size = size;

  // allocate  
  _puzzle = new int*[_size];
  for (int i = 0; i < _size; i++){
    _puzzle[i] = new int[_size];
  }
  
    for(int i = 0; i < _size; i++) {
      for (int j = 0; j < _size; j++) {
        _puzzle[i][j] = i * _size + j;
      }
    }
    _row_index = 0;
    _col_index = 0;
    _puzzle[_row_index][_col_index] = -1;
  
    // randomize
    for (int i = 0; i < 1000; i++) {
      move(rand() % 4);
    }
	//FILE* fp = fopen(fname, "w");
	fprintf(fp, "%d ", _size);
    for(int i = 0; i < _size; i++) {
      for (int j = 0; j < _size; j++) {
        fprintf(fp, "%d ", _puzzle[i][j]);
      }
    }
    //fclose(fp);
}

void read(const char* fname) {
	char fn[100];
	sprintf(fn, "%s.txt", fname);
	FILE* fp = fopen(fn, "r");
	//fscanf(fp, "%d", &_size);

  // allocate  
  _puzzle = new int*[_size];
  for (int i = 0; i < _size; i++){
    _puzzle[i] = new int[_size];
  }
    for(int i = 0; i < _size; i++) {
      for (int j = 0; j < _size; j++) {
        fscanf(fp, "%d", &_puzzle[i][j]);
        if (_puzzle[i][j] == 0) {
          _puzzle[i][j] = -1;
		  _row_index = i;
		  _col_index = j;
		}
      }
    }
	fscanf(fp, "%d", &OPT);
    
    fclose(fp);
}

void read_test(const char* fname) {
	char fn[100];
	sprintf(fn, "%s.puz", fname);
	FILE* fp = fopen(fn, "r");
	fscanf(fp, "%d", &_size);

  // allocate  
  _puzzle = new int*[_size];
  for (int i = 0; i < _size; i++){
    _puzzle[i] = new int[_size];
  }
    for(int i = 0; i < _size; i++) {
      for (int j = 0; j < _size; j++) {
        fscanf(fp, "%d", &_puzzle[i][j]);
        if (_puzzle[i][j] == -1) {
		  _row_index = i;
		  _col_index = j;
		}
      }
    }
    
    fclose(fp);
}

void reset() {
  for (int i = 0; i < _size; i++) {
	delete _puzzle[i];
  }
  delete _puzzle;
}
