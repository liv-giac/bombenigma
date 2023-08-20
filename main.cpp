
#include "enigma.h"
#include "alphabet.h"
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

int main(int argc, char** argv){
//i need to provide at least a plugboard and reflector file, and if i am providing rotors i need to also provide the position configuration file
  if(argc < 3 || argc == 4) cout << "Wrong number of arguments" << endl;
  Enigma *enigma = nullptr;

  char letter;
  // Within this while loop,
  // 1. it extract input letter by letter,
  // 2. check if it is within uppercase alphabet in ASCII table character code
  // 3. Encrypt a letter and overwrite it.
  while(!cin.eof()){
    cin >> letter;
    if(cin.fail()){
      break;
    }

    if(letter - 'A' < 0 || ALPH_LEN -1 < letter - 'A'){
      // This line exceeds 80 characters, but if I break it into two lines,
      // I cannnot pass LabTS auto-tests
      cout << letter << " is not a valid input character (input characters must be upper case letters A-Z)!" << endl;
      delete enigma;
      
    }
    enigma->encryptMessage(letter);
    cout << letter;
  }
  delete enigma;
  return 0;
}
