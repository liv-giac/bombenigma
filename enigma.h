#ifndef ENIGMA_H
#define ENIGMA_H

#include "plug_reflect.h"
#include "rotor.h"
#include <fstream>
#include <vector>

//using namespace std;

class Enigma{
private:
vector<Rotor> rotors; //the rotors 
vector<int> rotor_positions; //the offsets
int num_of_rotors; //useful to have as a stored int
Plug_reflect *plugboard{nullptr};
Plug_reflect *reflector{nullptr};
public:
  //constructor
  Enigma(int argc, char** argv);
  Enigma(vector<int> rotor_indexes, vector<int> rotor_pos, vector<int> plugboard, int reflector_index);
  //checks plugboard mappings and prints any errors
  void PlugboardConfig(const char* path, vector<int>& contacts);
  //the bombe needs this to initiate a plugboard from an array of 4 integers
  void PlugboardConfig2(const char* path, vector<int>& contacts);
  //checks reflector mappings and prints any errors
  void ReflectorConfig(const char* path, vector<int>& contacts);
  //checks the rotor contacts and prints any errors
  void RotorConfig(const char* path, vector<int>& contacts);
  // saves the rotor positions in the right file and prints any errors
  void RotorPositionConfig(const char* path); 
  //a helper method to check if the values (read in pairs, this is why it's useful) are valid and can be read for the plugboard configuration
  bool PlugboardCheck(const char* path, fstream& in_stream, int& index_num);
  // check if num is in the range 0-25
  bool RangeCheck(int num);
  // this checks if the value num has already appeared before in the contacts, returns index if yes, otherwise returns -1
  int AppearedBefore(vector<int> contacts, int num, int position);
  //rotates the rotors without encoding and returns the new position after the time
  void timetravel(int time);
  // the encryption method
  void encryptMessage(char& letter);
  void printEnigma();
};

#endif
