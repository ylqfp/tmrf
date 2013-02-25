/**
 * bag-of-biterms topic model(B^2TM) with Gbbis sampling implementation
 * Author: Xiaohui Yan(xhcloud@gmail.com)
 * 2012-11-29
 */
#ifndef _MODEL_H
#define _MODEL_H

#include <vector>

#include "biterm.h"
#include <pvec.h>
#include <utility>

using namespace std;

class Model {
protected:
  int N;				// biterm number
  int M;				// word number
  int K;				// topic number
  int n_iter;
  int save_step;

  double alpha;			// hyperparameters of p(z)
  double beta;			// hyperparameters of p(w|z)
  double gamma;			// p(zi==zj)
  bool weighted;        // distance weighted dependence of biterms
  
  string dir;			// result dir
  string dfile;			// training file
  
  vector< vector<Biterm> > docs;		// training docs represented by bag-of-biterms

  // sample recorders
  vector< vector<pair<int, int> > > _dbz;	// topic assignment for each (d,b) occurrence
  vector< vector<int> > _nz_d;	// n(d,z), size N*K
  vector< vector<int> > _nwz;	// n(w,z), size M*K
  vector<int> _nw_z;			// n(w|z)=\sum_w{n(w,z)}, size K*1

 public:
  Model(int K, double a, double b, double c, bool weighted, int n_iter, int save_step, const string& dfile, const string& dir):
	K(K), alpha(a), beta(b), gamma(c), dfile(dfile), dir(dir), n_iter(n_iter), save_step(save_step), weighted(weighted) {}
  
  // run estimate and inference procedures
  void run();
  
 protected:
  void load_docs();

  
  // allocate varibles
  virtual void init()=0;
  
  void rand_init();				// random initalize varibles
  void estimate();

  // update estimate of a biterm
  void update_biterm(int d, int b);
  
  // reset topic proportions for biterm b
  void reset_biterm_topic(int d, int b);
  
  // assign topic proportions for biterm b
  void assign_biterm_topic(int d, int b, int k1, int k2);
  
  // compute condition distribution p(z|b)
  void compute_pz_db(int d, int b, Pvec<double>& p); 
  void compute_pz_dw(int d, int i, Pvec<double>& p);
  
  void store_pz_d();
  void store_pw_z();
  void store_dbz();
};

#endif
