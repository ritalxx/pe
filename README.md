# Solving The Probability Of Evidence Problem

Bayesian network is a probabilistic graphical model that represents a set of random variables and their conditional dependencies. One of the common task is to compute the probability of given evidences, which is usually denoted as P(e).

## How to compile

I use boost regex library (Regular Expression) to extract data from Xbif files (download and compile [boost](http://www.boost.org) library). To get the best performance, we use gcc level 3 optimization to compile the code:
```
g++ −I/path/to/boost_1_57_0/ −I./ −O3 −L/path/to/boost_1_57_0/stage/lib/ −std=c++11 ∗.cpp 
−o exact_pe −lboost regex
```
