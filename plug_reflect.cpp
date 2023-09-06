#include "plug_reflect.hpp"
#include "alphabet.hpp"
#include <iostream>
#include <fstream>
using namespace std;

Plug_reflect::Plug_reflect(const char* path){
    fstream in_stream;
  in_stream.open(path);
  int num;
  int count = 0;
  while(in_stream >> num){
    if(count < ALPH_LEN && count%2){
      input_pair1.push_back(num);
    }
    else if(count < ALPH_LEN){
        input_pair2.push_back(num);
    }else break;
    count++;
    }
    in_stream.close();
  }

Plug_reflect::Plug_reflect(vector<int> input){

  int count = 0;
  for (int num : input){
    if(count < ALPH_LEN && count%2){
      input_pair1.push_back(num);
    }
    else if(count < ALPH_LEN){
        input_pair2.push_back(num);
    }else break;
    count++;
    }
    
  }

  std::vector<int> Plug_reflect::getpair1(){
    return input_pair1;
  }
  std::vector<int> Plug_reflect::getpair2(){
    return input_pair2;
  }
int Plug_reflect::map(int input){
    //they are the same length
    int length = input_pair1.size();
for (int i=0; i<length; i++){
    if (input==input_pair1[i])
    return input_pair2[i];
    if (input==input_pair2[i])
    return input_pair1[i];
}
return input;
}

