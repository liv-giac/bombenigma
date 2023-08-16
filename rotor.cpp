#include "rotor.h"
#include "alphabet.h"
#include <iostream>
#include <fstream>

using namespace std;

//constructor reads the rotor connections and saves them in contacts, also saves the notch position
Rotor::Rotor(const char* path, int start_position){
  fstream in_stream;
  in_stream.open(path);
  int num;
  int count = 0;
  while(in_stream >> num){
    if(count < ALPHABET_LENGTH){
      contacts[count] = num;
    }
    else{
      notch=num;
      break;
    }
    count++;
  }
  curr_pos= start_position;
}

//getter
int Rotor::getPreviousPosition(){
  return prev_pos;
}
//getter
int Rotor::getCurrentPosition(){
  return curr_pos;
}

//simple rotation, saving the position in prev_pos and updating, with modulus to move from 25 to 0
void Rotor::rotate(){
  prev_pos = curr_pos;
  curr_pos = (curr_pos + 1) % ALPH_LEN;
}

//in the shifts we use the modulus to make sure we don't exit the range 0-25

//shift up is used to shift to the offset position of the rotor (curr_pos) and return to "standard" working index
int Rotor::shiftUp(int input_index){
  return (input_index - getCurrentPosition() + ALPH_LEN) \
  % ALPH_LEN;
}
//shift down is used to shift from the offset position of the rotor (curr_pos) and return to "standard" working index
int Rotor::shiftDown(int input_index){
  return (input_index + getCurrentPosition()) % ALPH_LEN;
}

int Rotor::mapForward(int input_index){
  return contacts_[input_index];
}

int Rotor::mapBackward(int contact){
  for(int i = 0; i < ALPHABET_LENGTH; i++){
      if(contact == contacts_[i]){
          return i;
      }
  }
  return contact;
}

bool Rotor::isCurrentPositionInNotch(){
  int num_of_notches = notches_.size();
  for(int i= 0; i< num_of_notches; i++){
    if(current_position_ == notches_[i]){
      return true;
    }
  }
  return false;
}
