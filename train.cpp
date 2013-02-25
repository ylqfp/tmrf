#include <iostream>
#include "train.h"

// n_status: number of latent status for each word
void Train::init() {
  cout << "train init" << endl;
  load_docs();
  N = docs.size();
  // find the maximum word id
  int max_w = -1;
  for (int n = 0; n < N; ++n) {
	  for (int i = 0; i < docs[n].size(); ++i) {
		  if (max_w < docs[n][i].second()) max_w = docs[n][i].second();
	  }
  }

  M = max_w + 1;
  cout << "N=" << N << ", M=" << M << endl;
  
  _nz_d.resize(N);
  for (int n = 0; n < N; ++n)
	_nz_d[n].resize(K);
  
  _nw_z.resize(K);
  _nwz.resize(M);
  for (int m = 0; m < M; ++m)
	_nwz[m].resize(K);

  _dbz.resize(N);
  for (int n = 0; n < N; ++n)
	_dbz[n].resize( docs[n].size() );
}
