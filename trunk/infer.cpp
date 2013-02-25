#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include "infer.h"

void Infer::init() {  
  cout << "infer init" << endl;
  load_docs();
  N = docs.size();
  
  _nw_z.resize(K);
  // initialize _nw_z and _nwz from last assignment
  load_model();		
  M = _nwz.size();
  cout << "N=" << N << ", M=" << M << endl;

  _dbz.resize(N);
  for (int n = 0; n < N; ++n)
	_dbz[n].resize( docs[n].size() );

  _nz_d.resize(N);
  for (int n = 0; n < N; ++n)
	_nz_d[n].resize(K);
}

// load biterm topic assignments output by training precess
// each line is a doc, format:w1-w2:topic_w1-topic_w2 ...
// set topic recorder _nwz and _nw_z
void Infer::load_model() {
  ostringstream ss;
  ss << "dbz.k" << K;
  string pt = train_dir + ss.str();
  
  cout << "->load dbz from:" << pt << endl;
  ifstream rf( pt.c_str() );
  if (!rf) {
	EXIT_ERR("file not find:", pt.c_str());
  }

  string line;
  while(getline(rf, line)) {
	// read a doc
    istringstream iss(line);
	
	int w1, w2, k1, k2;
	char c1, c2, c3;
    while (iss >> w1 >> c1 >> w2 >> c2 >> k1 >> c3 >> k2) {
		// allocacte _nwz
	  while (_nwz.size() < w2+1) {
		vector<int> tmp(K);
		_nwz.push_back(tmp);
	  }
	  assert(k1 < K && k2 < K);
	  _nwz[w1][k1]++;
	  _nw_z[k1]++;
	  _nwz[w2][k2]++;
	  _nw_z[k2]++;
	}
  }

  printf("M=%d\n", _nwz.size());
}
