#ifndef PLUG_REFLECT_H
#define PLUG_REFLECT_H

#include <vector>

// plugboards and reflectors work in a very similar way when it comes to mapping 

class Plug_reflect{
private:
  //we create two vectors to store the mapped pairs
  std::vector<int> input_pair1;
  std::vector<int> input_pair2;
public:
  //this constructor reads the pairs from the file and stores them in the two vectors
  Plug_reflect(const char* path);
  //this simply maps the imput according to the pairs.
  int map(int input);
};

#endif
