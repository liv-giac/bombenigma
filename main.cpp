#include "enigma.h"
#include "alphabet.h"
#include <iostream>
#include <cstring>
#include <string>
#include "encodetools.h"

using namespace std;

int main(int argc, char** argv){
  //first we encode the message knowing "today's settings" and the original message
string output = encodeFullMessage(argc, argv);
//now we pretend to forget the initial settings and message, and only know the coded message. we will have to work backwards.
//first we read the key from file
string key= getKeyFromFile();
//then we need to find in the message a crib: a string of the same length of the key where there are no letters that correspond to itself
vector<string>  cribList = getCribs(output, key);
int n_cribs=cribList.size();
if (n_cribs==0) {cout << "There are no cribs, so our Bombe cannot decode this message."; return 0; }
vector<codePair> loop;
for (string crib : cribList){
  set<codePair> codePairs = readMatches(key,crib);  
  vector<codePair> loop = findLoop(codePairs);
  if (!loop.empty()) {
    cout << "Loop of three pairs of letters found in the crib " << crib <<": " ;
      for (const codePair& pair : loop) {
        cout << "(" << pair.letter1 << ", " << pair.letter2 << ", "<< pair.pos <<") ";
      }
      cout << endl;
      break;
      } else {
          cout << "No loop of three pairs of letters found in crib: " << crib<< endl; 
             }
}
if (loop.empty()){

}


  return 0;
}