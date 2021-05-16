/*
 * Author: Satya Gautam Vadlamudi (satya AT cse DOT iitkgp DOT ernet DOT in; vsatyagautam AT gmail DOT com)
*/

void CreateNodes (FILE * fp, int boundary) {
  fprintf (fp, "%10d %10d\n", nodes, boundary);
  for (int j = 0; j < nodes; j++) {
    double x = boundary * (rand() / (RAND_MAX + 1.0));
    double y = boundary * (rand() / (RAND_MAX + 1.0));
    fprintf (fp, "%d %lf\t %lf\n", j + 1, x, y);
  }
}

int GetDistance(int i, int j) {
  if (i == j) {
	return 0;
  }
  if (i < j) {
	return dist[i][j - i - 1];
  }
  return dist[j][i - j - 1];
}

// Only for symmetric.
void ComputeDistance (double* x, double* y, int topnode, int newnode, bool isGeo, bool isCeil, bool isATT) {
  /*if (topnode == newnode) {
	return 0;
  }*/
  assert(topnode < newnode);
  double x1 = x[topnode];
  double y1 = y[topnode];
  double x2 = x[newnode];
  double y2 = y[newnode];/*
  if (topnode > newnode) {
	int t = topnode;
	topnode = newnode;
	newnode = t;
  }*/
  
  if (isATT) {
	int xd = x1 - x2;
	int yd = y1 - y2;
	double r = sqrt((xd * xd + yd * yd) / 10.0);
	int t = int(r + 0.5);
	if (t < r) {
      dist[topnode][newnode - topnode - 1] = t + 1;	  
	} else {
      dist[topnode][newnode - topnode - 1] = t;
	}
	return;
  }
  
  if (isGeo) {
    double RRR = 6378.388;
    double q1 = cos( y1 - y2 );
    double q2 = cos( x1 - x2 );
    double q3 = cos( x1 + x2 );
    dist[topnode][newnode - topnode - 1] = (int)(RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0); 
    return;
  }
  
  double x_2 = pow (x1 - x2, 2);
  double y_2 = pow (y1 - y2, 2);
  if (isCeil) {
    dist[topnode][newnode - topnode - 1] = ceil(sqrt(x_2 + y_2));
  } else {
    dist[topnode][newnode - topnode - 1] = int(sqrt(x_2 + y_2) + 0.5);
  }
}

void ReadInput(FILE* fp, bool isGeo, bool isCeil, bool isATT) {
  //if (!atsp) {
  double x[nodes];
  double y[nodes];
  //double* y = new double[nodes];
  for (int m = 0; m < nodes; m++) {
	int t;
    fscanf (fp, "%d", &t);
    assert(m + 1 == t);
    double mx, my;
    fscanf (fp, "%lf", &mx);
    fscanf (fp, "%lf", &my);
    if (isGeo) {
      double PI = 3.141592;
      int deg = int(mx);
      double min = mx- deg;
      mx = PI * (deg + 5.0 * min/ 3.0) / 180.0;
      deg = int(my);
      min = my- deg;
      my = PI * (deg + 5.0 * min/ 3.0) / 180.0;
	}
	x[m] = mx;
	y[m] = my;
  }
  for (int i = 0; i < nodes; i++) {
	for (int j = 0; j < i; j++) {
	  ComputeDistance(x, y, j, i, isGeo, isCeil, isATT);
	}
  }/*
  delete x;
  delete y;*/
  /*
	} else {
	  for (int i = 0; i < nodes; i++) {
		for (int j = 0; j < nodes; j++) {
		  fscanf(fp, "%lf", &dist[i][j]);
		}
	  }
	}*/
}

void ReadFullMatrix(FILE* fp) {
  for (int i = 0; i < nodes; i++) {
	for (int j = 0; j < i; j++) {
	  fscanf(fp, "%d", &dist[j][i - j - 1]);
	}
	for (int j = i; j < nodes; j++) {
	  fscanf(fp, "%*d");
    }
  }
}

void ReadLowerDiag(FILE* fp) {
  for (int i = 0; i < nodes; i++) {
	for (int j = 0; j < i; j++) {
	  fscanf(fp, "%d", &dist[j][i - j - 1]);
	}
	fscanf(fp, "%*d");
  }
}

void ReadUpperDiag(FILE* fp) {
  for (int i = 0; i < nodes; i++) {
	fscanf(fp, "%*d");
	for (int j = i + 1; j < nodes; j++) {
	  fscanf(fp, "%d", &dist[i][j - i - 1]);
	}
  }
}

void ReadUpper(FILE* fp) {
  for (int i = 0; i < nodes; i++) {
	for (int j = i + 1; j < nodes; j++) {
	  fscanf(fp, "%d", &dist[i][j - i - 1]);
	}
  }
}

void read(const char *a, bool isGeo, bool readLowerDiag) {
  char fname[100];
  sprintf(fname, "%s.tsp", a);
  int boundary;
  FILE* fp = fopen (fname, "r");
  fscanf (fp, "%d", &nodes);
  fscanf (fp, "%d", &boundary);
  dist = new int*[nodes - 1];
  for (int i = 0; i < nodes - 1; i++) {
	dist[i] = new int[nodes - i - 1];
  }
  if (readLowerDiag) {
    ReadLowerDiag(fp);	
  } else {
    ReadInput(fp, isGeo, 0, 0);
  }
  fclose(fp);
}

void read(const char *b) {
  char fname[100];
  sprintf(fname, "input_benchmarks/%s.tsp", b);
  char a[100];
  FILE* fp = fopen (fname, "r");
  fscanf (fp, "%d", &nodes);
  fscanf (fp, "%s", a);
  printf("-------------------------------------\n");
  printf("%s %s\n", fname, a);
  dist = new int*[nodes - 1];
  for (int i = 0; i < nodes - 1; i++) {
	dist[i] = new int[nodes - i - 1];
  }
  if (strcmp(a, "EUC_2D") == 0) {
    ReadInput(fp, 0, 0, 0);
  }	else if (strcmp(a, "CEIL_2D") == 0) {
    ReadInput(fp, 0, 1, 0);
  } else if (strcmp(a, "GEO") == 0) {
    ReadInput(fp, 1, 0, 0);
  } else if (strcmp(a, "LOWER_DIAG_ROW") == 0) {
    ReadLowerDiag(fp);
  } else if (strcmp(a, "UPPER_DIAG_ROW") == 0) {
    ReadUpperDiag(fp);
  } else if (strcmp(a, "UPPER_ROW") == 0) {
    ReadUpper(fp);
  } else if (strcmp(a, "FULL_MATRIX") == 0) {
    ReadFullMatrix(fp);
  } else if (strcmp(a, "ATT") == 0) {
    ReadInput(fp, 0, 0, 1);
  } else {
    printf("%s reading errror\n", a);
	exit(0);
  }
  fclose(fp);
}
