#ifndef _BITERM_H
#define _BITERM_H

#include <cmath>
using std::min;
using std::max;

class Biterm {
private:
  int wi;
  int wj;
  int freq;						// freqency
public:
  bool related;					// if true, the two words show the same topic, else independent
  
public: 
  Biterm(){}
  Biterm(int w1, int w2, int freq):freq(freq) {
	wi = min(w1, w2);
	wj = max(w1, w2);
  }

  Biterm(int w1, int w2, int freq, bool related):freq(freq), related(related) {
	wi = min(w1, w2);
	wj = max(w1, w2);
  }

  int count() const {return freq;}
  int first() const {return wi;}
  int second() const {return wj;}
  
};

#endif
