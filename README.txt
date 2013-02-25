Implementation of the topic-based Markov random field(TMRF) model
More detail is refered to the paper[1].

Author: Xiaohui Yan(xhcloud@gmail.com)
Date: 2012-09-25
Version: 0.5

1. Implentation
The code impletement the Pairwise TMRF with the edge independence
assumption that each word pair within a same context are independent. 
In such case, each document is viewed as a bag of word pairs (or
called biterms), which co-occurred closely. The distribution of a
biterm b=(w1,w2) in document d is
   
   P(w1,w2|d) = \sum_z{P(w1|z)P(w2|z)P(z|d)}.

Steps of Gibbs algorithm for TMRF:
1) Randomly assign topic uniformly to each biterm b in all documents
2) for each document d
   a) for each biterm b in d
      i) reset topic assignment of b
   	  ii) sample topic k according to P(z|d-b)
	  iii) re-assign topic z to biterm b
3) looper step 2) until converge
4) inference the parameters {P(z|d), P(w|z)}

TODO:

History：

References
[1] X. Yan, J. Guo, Y. Lan, and X. Cheng. Topic-Based Markov Random Field: Beyond Bag-of-words
Topic Modeling. Submit to KDD'13

