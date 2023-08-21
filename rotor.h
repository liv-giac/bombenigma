#ifndef ROTOR_H
#define ROTOR_H

#include "alphabet.h" //alph length
#include <vector>

using namespace std;

class Rotor{
private:
  //this is the offset of the rotor compared to letter A
  int curr_pos;
  // with this position we compare how much we have rotated and if is time to trigger another rotor
  int prev_pos;
  // here we store the first 26 letters 
  int contacts[ALPH_LEN];
  // here we read the last number as the position of the notch
  int notch;

public:
  // when constructing we will get the positions of the letters on rotors and the notch from the file and assign them to contacts and notch
  // we also read the current_position from file
  Rotor(const char* path, int start_position);
  // getters
  int getCurrentPosition();
  int getPreviousPosition();
  // updates curr_pos and prev_pos
  void rotate();
  // shiftUp and shiftDown calculates absolute position of input_index
  // by adding/subtructing current position of rotor.
  int shiftUp(int input_index);
  int shiftDown(int input_index);
  // returns the output reading from the contacts
  int mapForward(int input_index);
  // returns the index in which the contact is in
  int mapBackward(int contact);
  // returns true if curr_pos is at the notch
  bool isItNotch();
  vector<int> getNotchAndPos();
  vector<int> getContacts();
};

#endif
