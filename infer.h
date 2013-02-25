#ifndef _INFER_H
#define _INFER_H

#include <string>
#include "model.h"

using namespace std;

class Infer:public Model {
private:
  string train_dir;
public:
  Infer(int K, double a, double b, double c, bool weighted, int n_iter, int save_step,
		const string& dfile, const string& dir, const string& train_dir):
	Model(K, a, b, c, weighted, n_iter, save_step, dfile, dir), train_dir(train_dir) {}

private:
  void init();
  void load_model();
};

#endif
