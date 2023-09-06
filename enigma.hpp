#ifndef ENIGMA_H
#define ENIGMA_H

#include "plug_reflect.hpp"
#include "rotor.hpp"
#include <fstream>
#include <vector>

//using namespace std;

class Enigma{
private:
vector<Rotor> rotors; //the rotors 
vector<int> rotor_positions; //the offsets
int num_of_rotors; 
Plug_reflect *plugboard{nullptr};
Plug_reflect *reflector{nullptr};
public:
  //constructor from makefile/shell info
  Enigma(int argc, char** argv);
  //constructur from inline input data
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
  //sets rotor positions from given integers
  void set_pos(vector<int> pos);
  // the encryption method
  void encryptMessage(char& letter);
  //prints enimga data for debugging
  void printEnigma();
};

#endif
