#ifndef _TRAIN_H
#define _TRAIN_H

#include <string>
#include "model.h"

using namespace std;

class Train: public Model {
public:
  Train(int K, double a, double b, double c, bool weighted, int n_iter, int save_step, const string& dfile, const string& dir):
	Model(K, a, b, c, weighted, n_iter, save_step, dfile, dir) {}
  
private:
  void init();
};
#endif
