# Solving the Probability of Evidence Problem

## How to compile

I use boost regex library (Regular Expression) to extract data from Xbif files (download and compile [boost](http://www.boost.org) library). To get the best performance, we use gcc level 3 optimization to compile the code:
```
g++ −I/path/to/boost_1_57_0/ −I./ −O3 −L/path/to/boost_1_57_0/stage/lib/ −std=c++11 ∗.cpp 
−o exact_pe −lboost regex
```
