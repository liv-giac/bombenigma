#ifndef PLUGREFLECT_H
#define PLUGREFLECT_H

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
  //this simply maps the input according to the pairs and returns
  int map(int input);
};

#endif
