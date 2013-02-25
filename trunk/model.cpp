#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include <ctime>
#include <algorithm>

#include <algorithm>
#include <sampler.h>
#include "model.h"
#include <pmat.h>

void Model::run() {
  init();
  
  cout << "begin estimate" << endl;
  estimate();
  
  cout << "store parameters" << endl;
  store_pz_d();
  store_pw_z();
  store_dbz();
}

// input format:did   w1    w2    freq
void Model::load_docs() {
  printf("train docs:%s\n", dfile.c_str());

  ifstream rf( dfile.c_str() );
  if (!rf) 	EXIT_ERR("file not find:", dfile.c_str());
  
  string line;
  int max_wid = -1;
  while ( getline(rf, line) ) {
	int did, w1, w2, freq;
	bool related;
	istringstream iss(line);
	iss >> did >> w1 >> w2 >> freq >> related;

	max_wid = max(max(w1, w2), max_wid);
	Biterm bi = Biterm(w1, w2, freq, related);
	
	// add to doc
	if (did >= docs.size()) {
	  vector<Biterm> doc(1, bi);
	  docs.push_back(doc);
	} else {
	  docs[did].push_back(bi);
	}
  }
}

void Model::estimate() {
  srand(time(NULL));
  rand_init();
  
  //string sdir = dir;
  for (int iter=1; iter < n_iter+1; ++iter) {
	cout << "iter " << iter << endl;
	for (int d=0; d < N; ++d)
	  for (int b=0; b < docs[d].size(); ++b)
		update_biterm(d, b);

	if (iter % save_step ==0) {
	  cout << "store parameters" << endl;
	  // for convergence test
	  //  ostringstream ss;
	  // ss << sdir << iter << "_";
	  // dir = ss.str();
	  
	  store_pz_d();
	  store_pw_z();
	  store_dbz();
	}
	  
  }
  cout << "Gibbs completed!" << endl;
}

void Model::rand_init() {
  // init sample
  for (int d=0; d < N; ++d) {
	for (int b=0; b < docs[d].size(); ++b) {
	  // sample for topics for each biterm
	  //int s = Sampler::Bern_sample(gamma);
	  
		int k1, k2;
		k1 = Sampler::uni_sample(K);
		if (docs[d][b].related) 		// the same topic
			k2 = k1;
		else              // the different topic
			k2 = Sampler::uni_sample(K);
	  
		assign_biterm_topic(d, b, k1, k2);
	}
  }
}

// sample procedure for biterm b
void Model::update_biterm(int d, int b) {
  reset_biterm_topic(d, b);

  Biterm& bi = docs[d][b];
  //double p = gamma;
  //if (weighted) p /= bi.distance();

  //int s = Sampler::Bern_sample(p);
  if (bi.related) {
	// sample topic for biterm b  
	Pvec<double> p;
	compute_pz_db(d, b, p);
	int k = Sampler::mult_sample(p.to_vector());
	
	assign_biterm_topic(d, b, k, k);
  } else {
	int w1 = bi.first();
	int w2 = bi.second();

	// independent sample topic for each words
	Pvec<double> p1, p2;
	compute_pz_dw(d, w1, p1);
	int k1 = Sampler::mult_sample(p1.to_vector());

	compute_pz_dw(d, w2, p2);
	int k2 = Sampler::mult_sample(p2.to_vector());

	assign_biterm_topic(d, b, k1, k2);
  }
}

// reset topic assignment of biterm i
void Model::reset_biterm_topic(int d, int b) {
  Biterm& bi = docs[d][b];
  int w1 = bi.first();
  int w2 = bi.second();
  int t = bi.count();
  
  int k1 = _dbz[d][b].first;
  int k2 = _dbz[d][b].second;

  _nz_d[d][k1] -= t;
  _nz_d[d][k2] -= t;
  
  _nw_z[k1] -= t;				// update proportion of words in topic k
  _nw_z[k2] -= t;				// update proportion of words in topic k
  
  _nwz[w1][k1] -= t;			// update proportion w1's occurrence times in topic K
  _nwz[w2][k2] -= t;

  assert(_nz_d[d][k1] >=0 && _nz_d[d][k2] >=0 && _nwz[w1][k1]>=0 && _nwz[w2][k2]>=0);
}

// compute p(z|b, d)
void Model::compute_pz_db(int d, int b, Pvec<double>& p) {
  p.resize(K);
  Biterm& bi = docs[d][b];
  
  int w1 = bi.first();
  int w2 = bi.second();
  double pw1k, pw2k;

  for (int k = 0; k < K; ++k) {
	double deno = _nw_z[k] + M * beta;
	
	pw1k = (_nwz[w1][k] + beta) / deno;
	pw2k = (_nwz[w2][k] + beta) / deno;
	p[k] = (_nz_d[d][k] + alpha)*(_nz_d[d][k] + alpha) * pw1k * pw2k;
  }
  p.normalize();
}

// compute p(z|d, w)
void Model::compute_pz_dw(int d, int w, Pvec<double>& p) {
  p.resize(K);
  
  for (int k = 0; k < K; ++k) {
	p[k] = (_nz_d[d][k] + alpha) * (_nwz[w][k] + beta) / (_nw_z[k] + M * beta);
  }
  p.normalize();
}

// assign topic k to biterm i
void Model::assign_biterm_topic(int d, int b, int k1, int k2) {
  Biterm& bi = docs[d][b];
  int w1 = bi.first();
  int w2 = bi.second();
  int t = bi.count();
	
  _dbz[d][b] = make_pair(k1, k2);
  _nz_d[d][k1] += t;
  _nz_d[d][k2] += t;
  
  _nw_z[k1] += t;
  _nw_z[k2] += t;
  
  _nwz[w1][k1] += t;
  _nwz[w2][k2] += t;
}

// store as N*K matrix
void Model::store_pz_d() {
  ostringstream ss;
  ss << "zd.k" << K;
  string suf = ss.str();
  string pt = dir + suf;
  
  ofstream wf(pt.c_str());
  if (!wf) {
	EXIT_ERR("file not find:", pt.c_str());
  }
  cout << "writing p(z|d): " << pt << endl;

  for (int d=0; d < _nz_d.size(); ++d) {
	Pvec<double> pz(K);	          // p(z) = theta
	for (int k = 0; k < K; k++) {
	  pz[k] = (_nz_d[d][k] + alpha);
	}
	pz.normalize();
	if (d % 100000 ==0)
	  cout << "write " << d << '/' << docs.size() << endl;
	wf << pz.str(false) << endl;
  }
}

void Model::store_pw_z() {
  //void Model::write_pw_z()
  ostringstream ss;
  ss << "zw.k" << K;
  string suf = ss.str();
  string pt = dir + suf;
  
  ofstream wf(pt.c_str());
  if (!wf) {
	EXIT_ERR("file not find:", pt.c_str());
  }
  cout << "writing p(w|z): " << pt << endl;

  for (int k = 0; k < K; k++) {
	Pvec<double> pw_z(M);   
	double deno = _nw_z[k] + M * beta;
	
	for (int m = 0; m < M; m++) 
	  pw_z[m] = (_nwz[m][k] + beta) / deno;
	
	wf << pw_z.str(false) << endl;
  }
}

// store topic assignment for each biterm
// format:w1-w2:topic_w1-topic_w2
void Model::store_dbz() {
  ostringstream ss;
  ss << "dbz.k" << K;
  string suf = ss.str();
  string pt = dir + suf;
  
  ofstream wf(pt.c_str());
  if (!wf) {
	EXIT_ERR("file not find:", pt.c_str());
  }
  cout << "write dbz: " << pt << endl;

  for (int d=0; d < N; ++d) {
	for (int i=0; i < _dbz[d].size(); ++i) {
	  Biterm& bi = docs[d][i];
	  int w1 = bi.first();
	  int w2 = bi.second();
	  
	  wf << w1 << '-' << w2 << ':' << _dbz[d][i].first << "-" << _dbz[d][i].second << " ";
	}
	wf << endl;
  }
}
