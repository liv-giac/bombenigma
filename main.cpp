#include "enigma.h"
#include "alphabet.h"
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

int main(int argc, char** argv){

//i need to provide at least a plugboard and reflector file, and if i am providing rotors i need to also provide the position configuration file
  if(argc < 3 || argc == 4) cout << "Wrong number of arguments" <<endl;
  char letter = 'A';
  Enigma *enigma = nullptr;
  enigma = new Enigma(argc, argv);
  
  while(cin >> letter){
    if(cin.fail()){
      break;
    }
    if(letter - 'A' < 0 || ALPH_LEN -1 < letter - 'A'){

      cout << letter << " is not a valid input character (input characters must be upper case letters A-Z)!" << endl;
      delete enigma;
      return 0;
    }
    char oldletter = letter;
    enigma->encryptMessage(letter);
    cout << oldletter << " has been decoded into " <<letter << endl << endl;
  }
  delete enigma;
  return 0;
}
