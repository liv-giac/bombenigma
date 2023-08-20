#include "enigma.h"
#include "plug_reflect.h"
#include "rotor.h"
#include "alphabet.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
Enigma::Enigma(int argc, char** argv){
  vector<int> plugboard_contacts;
  PlugboardConfig(argv[1],plugboard_contacts);
  vector<int> reflector_contacts;
  checkReflectorConfig(argv[2], reflector_contacts);
  vector<vector<int>> rotor_contacts_array;

  vector<int> rotor_contacts;
  for(int i = 3; i < argc-1; i++){
    checkRotorConfig(argv[i], rotor_contacts);
    rotor_contacts_array.push_back(rotor_contacts);
    rotor_contacts.clear();
  }
  // 3 argc means no rotor is provided
  if(argc == 3){
    num_of_rotors = 0;
  }
  else{
    num_of_rotors = argc-4; 

  }
  RotorPositionConfig(argv[argc-1]);

  // All the checking is done at this point, so instantiate each component

  for(int i = 0; i < num_of_rotors; i++){
    Rotor rotor(argv[3+i], rotor_positions[i]);
    rotors.push_back(rotor);
  }
}

bool Enigma::PlugboardCheck(const char* path, fstream& in_stream, int& index_num){
  in_stream >> ws;
  if(in_stream.peek() == EOF){
    return false;
  }
  in_stream >> index_num;
  if(in_stream.fail()){
    cout << "Incompatible character in plugboard configuration file " << path << endl;
    in_stream.close();
  }
  if(!RangeCheck(index_num)){
    cout << "The plugboard configuration file " << path << \
    " contains a number not in 0-25 range" << endl;
    in_stream.close();
  }
  return true;
}

void Enigma::PlugboardConfig(const char* path, vector<int>& contacts){
  int even_index_num, odd_index_num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cout << "Error with the plugboard configuration file " << path << endl;
    in_stream.close();
  }
  while(!in_stream.eof()){
    if(!PlugboardCheck(path, in_stream, even_index_num)){
      break;
    }
    if(!PlugboardCheck(path, in_stream, odd_index_num)){
      cout << "Wrong number of parameters in the plugboard configuration file " << path << endl;
      in_stream.close();
    }
    contacts.push_back(even_index_num);
    if(AppearedBefore(contacts, even_index_num, counter) != -1){
      in_stream.close();
      cout << "Invalid plugboard configuration!" << endl;
    }
    contacts.push_back(odd_index_num);
    counter++;

    if(AppearedBefore(contacts, odd_index_num, counter) != -1){
      in_stream.close();
      cout << "Invalid plugboard configuration!" << endl;
    }
    counter++;
  }
  in_stream.close();
}

void Enigma::checkReflectorConfig(const char* path, vector<int>& contacts){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cerr << "Error opening or reading the configulation file " << path << endl;
    in_stream.close();
    throw(ERROR_OPENING_CONFIGURATION_FILE);
  }

  while(!in_stream.eof()){
    in_stream >> ws;
    int end_of_file = in_stream.peek();
    if(end_of_file == EOF){
      break;
    }
    in_stream >> num;
    if(in_stream.fail()){
      cerr << "Non-numeric character in reflector file " << path << endl;
      in_stream.close();
      throw(NON_NUMERIC_CHARACTER);
    }
    if(!isNumberRangeCorrect(num)){
      cerr << "The file " << path << \
      " contains a number that is not between 0 and 25" << endl;
      in_stream.close();
      throw(INVALID_INDEX);
    }
    contacts.push_back(num);
    if(counter < ALPHABET_LENGTH && \
      checkAppearedBefore(contacts, num, counter) != -1){
      in_stream.close();
      throw(INVALID_REFLECTOR_MAPPING);
    }
    counter++;

  }
  in_stream.close();

  if(counter%2!=0){
      cerr << "Incorrect (odd) number of parameters in reflector file " \
      << path << endl;
      throw(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
  }
  if(counter != ALPHABET_LENGTH){
    cerr << "Insufficient number of mappings in reflector file: " \
    << path << endl;
    throw(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
  }
}

void Enigma::checkRotorConfig(const char* path, vector<int>& contacts){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cerr << "Error opening or reading the configulation file " << path << endl;
    in_stream.close();
  }

  while(!in_stream.eof()){
    in_stream >> ws;
    int end_of_file = in_stream.peek();
    if(end_of_file == EOF){
      break;
    }
    in_stream >> num;

    if(in_stream.fail()){
      cerr << "Non-numeric character for mapping in rotor file " \
      << path << endl;
      in_stream.close();
      throw(NON_NUMERIC_CHARACTER);
    }
    if(!isNumberRangeCorrect(num)){
      cerr << "The file " << path \
      << " contains a number that is not between 0 and 25" << endl;
      in_stream.close();
      throw(INVALID_INDEX);
    }
    contacts.push_back(num);

    if(counter < ALPH_LEN-1 && \
      checkAppearedBefore(contacts, num, counter) != -1){
      in_stream.close();
      throw(INVALID_ROTOR_MAPPING);
    }
    counter++;
  }
  in_stream.close();

  if(counter < ALPH_LEN){
    cerr << "Not all inputs mapped in rotor file: " << path << endl;
    throw(INVALID_ROTOR_MAPPING);
  }
}

void Enigma::RotorPositionConfig(const char* path){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cout << "Error with the rotor configuration file " << path << endl;
    in_stream.close();
  }
  while(!in_stream.eof()){
    in_stream >> ws;
    if(in_stream.peek() == EOF) break;
     in_stream >> num;
    if(in_stream.fail()){
      cout << "Incompatible value in the rotor configuration file" << path  << endl;
      in_stream.close();
    }
    if(!RangeCheck(num)){
      cout << "The file " << path << " contains a number that is not between 0 and 25 CHANGE " << endl;
      in_stream.close();
    }
    counter++;
    rotor_positions.push_back(num);
  }

  if(counter - num_of_rotors < 0){
    cout << "Not enough starting positions provided in file " << path << endl;
    in_stream.close();
  }
  in_stream.close();
}

bool Enigma::RangeCheck(int num){
  return (num < ALPH_LEN && num >= 0);
}

int Enigma::AppearedBefore(vector<int> contacts, int num, int position){
  for(int i = 0; i < position; i++ ){
    if(contacts[i] == num){
      cout << "Value " << num << " is already mapped in position " << i << endl;
      return i;
    }
  }
  return -1;
}

void Enigma::encryptMessage(char& letter){
  int current_index = letter - 'A';
  current_index = plugboard_->map(current_index);

  if(num_of_rotors_ > 0){
    rotors_[num_of_rotors_-1].rotate();
  }

  if(num_of_rotors_ > 0){
    for(int i = num_of_rotors_ ; i > 0; i--){
      // TODO Needs explanation here
      current_index = rotors_[i-1].shiftDown(current_index);
      current_index = rotors_[i-1].mapForward(current_index);
      current_index = rotors_[i-1].shiftUp(current_index);
      if(rotors_[i-1].isCurrentPositionInNotch() && \
         rotors_[i-1].getPreviousPosition() != \
         rotors_[i-1].getCurrentPosition()){
        if(i-1 > 0){
          rotors_[i-2].rotate();
        }
      }
    }
  }
  current_index = reflector_->map(current_index);
  if(num_of_rotors_ > 0){
    for(int i = 0; i < num_of_rotors_; i++){
      current_index = rotors_[i].shiftDown(current_index);
      current_index = rotors_[i].mapBackward(current_index);
      current_index = rotors_[i].shiftUp(current_index);
    }
  }
  current_index = plugboard_->map(current_index);
  letter = current_index + 'A';
}
