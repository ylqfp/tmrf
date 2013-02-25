#include <cstdlib>
#include <string.h>
#include <string>
#include <cstdlib>
#include <string.h>
#include <string>
#include <iostream>
#include "train.h"
#include "infer.h"

using namespace std;

void usage() {
  cout << "Usage:\n"
       << "tmrf est K alpha beta pt_input pt_outdir n_iter\n"
       << "\tK\tint, number of topics, like 20\n"
       << "\talpha\tdouble, Pymmetric Dirichlet prior of P(z), like 1.0\n"
       << "\tbeta\tdouble, Pymmetric Dirichlet prior of P(w|z), like 0.01\n"
       << "\tpt_input\ttraining biterms file\n"
       << "\tpt_outdir\toutput directory\n"
       << "tmrf inf K pt_input pt_outdir\n"
       << "\tpt_input\tinference biterms file\n"
       << "\tpt_outdir\tparameters/output directory\n";
}

// void test_main() {
//    int K = 20;
//    double alpha = 0;
//    double beta = 0.01;
//    double gamma = 0.5;
//    string dfile = "./test/doc_biterms_w10.txt";
//    string dir = "./test/";
//    int n_iter = 10;
//    int save_step = 11;
//    Model model(K, alpha, beta, gamma, n_iter, save_step, dfile, dir);
//    model.run();
//    exit(0);
//  }

int main(int argc, char* argv[]) {
  //test_main();
  // load parameters from std input
  if (argc < 3) {
    usage();
    return 1;
  }

  int K = atoi(argv[2]);                  // topic num
  double alpha = atof(argv[3]);    // hyperparameters of p(z)
  double beta = atof(argv[4]);     // hyperparameters of p(w|z)
  double gamma = atof(argv[5]);
  bool weighted = atoi(argv[6]);
  int n_iter = atoi(argv[7]);
  int save_step = atoi(argv[8]);
  string dfile(argv[9]);
  string dir(argv[10]);

  cout << " ======== " << argv[0] << " " << argv[1] << " ======== " << endl;
  cout << "Para: K=" << K << ", alpha=" << alpha << ", beta=" << beta << ", gamma=" << gamma <<
	", weighted=" << weighted << ", n_iter=" << n_iter << ", save_step=" << save_step << endl;
	
    // load training data from file
  if (strcmp(argv[1], "est")==0) {
    Train model(K, alpha, beta, gamma, weighted, n_iter, save_step, dfile, dir);
    model.run();
  } else if (strcmp(argv[1], "inf")==0) {
	string train_dir(argv[11]);
    Infer model(K, alpha, beta, gamma, weighted, n_iter, save_step, dfile, dir, train_dir);
	model.run();
  }
}
