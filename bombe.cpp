#include "enigma.hpp"
#include "alphabet.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include "encodetools.hpp"

using namespace std;

int main(int argc, char** argv){
  //first we encode the messages knowing "today's settings" and the original messages
    vector<string> outputs = encodeFullMessages(argc, argv);

    for (string output : outputs){//we try each message
    cout << "The message we try to decode is " <<output << endl;
    //now we pretend to forget the initial settings and messages, and only know the coded messages. we will have to work backwards.
    //first we read the key from file
    string key= getKeyFromFile(); 
    //then we need to find in the message a crib: a string of the same length of the key where there are no letters that correspond to itself
    vector<string>  cribList = getCribs(output, key);
    int n_cribs=cribList.size();
    cout << "number of cribs found: " <<n_cribs<<endl;
    if (n_cribs==0) {cout << "There are no cribs, so our Bombe cannot decode this message."; return 0; }
    //if we have some suitable cribs, we can go on to search for a loop in the coupling of letters
    vector<codePair> loop;
    int cribPos;
    //we only need one loop so as soon as we find it we can stop
    for (string crib : cribList){//we try for all cribs
      cribPos=output.find(crib);
      set<codePair> codePairs = readMatches(key,crib,cribPos);
      loop = findLoop(codePairs);
      
      if (!loop.empty()) {
        cout << "Loop of three pairs of letters found in the crib " << crib <<": " ;
        
        cout << "(crib position "<<cribPos<< ") ";
          for (const codePair& pair : loop) {
            cout << "(" << pair.letter1 << ", " << pair.letter2 << ", "<< pair.pos <<") ";
          }
          cout << endl;
          }                 
    
    if (loop.empty()){
      cout << "No loops found in the crib "<<crib<<endl;
      continue;
      }
    //if we've found a loop, now it's time to use it to activate the actual bombe mechanism
  
    vector<int> rotor_indexes = {1, 2, 3};
    int reflector_index = 1;
    Enigma *enigma = startEnigma(rotor_indexes,{},reflector_index);//we start an enigma without plugboard settings
    string trial;

    int check=0;
    //we iterate through all the starting positions
     for (int i=1;i<ALPH_LEN;i++){
       for (int j=1;j<ALPH_LEN;j++){
        for (int k=1;k<ALPH_LEN;k++){
          trial={};
          trial+='A';
          enigma->set_pos({i,j,k});//we set the beginning position
          enigma->timetravel(loop[0].pos+cribPos);//the timetravel function sets the rotor to its position after x encodings
          trial+=encode(enigma,trial).back();
          enigma->set_pos({i,j,k});
          enigma->timetravel(loop[1].pos+cribPos);
          trial+=encode(enigma,trial).back();
          enigma->set_pos({i,j,k});
          enigma->timetravel(loop[2].pos+cribPos);
          trial+=encode(enigma,trial).back();
          if (trial.back()=='A') {
            //potential settings found!
            check=1;
            cout << "potential settings found with rotor positions "<<i<<" "<<j<<" "<<k<<"."<<endl<<"The plugboard settings would be: "<< endl;
            vector<char> loop_plugs={};
            loop_plugs.push_back(loop[0].letter2); //we select the three letters associated to the "wires"
            if (loop[0].letter2!=loop[1].letter1) loop_plugs.push_back(loop[1].letter1); else loop_plugs.push_back(loop[1].letter2);
            loop_plugs.push_back(loop[0].letter1);
            for (int m=0; m<loop_plugs.size();m++){ //we print the three found pairings
              cout << loop_plugs[m]<<" "<<trial[m]<< " // ";
            }
            cout<<endl;
            break;
          } 
        }
      }
   } if (check==0) cout << "No potential settings found with this crib."<<endl;
   delete enigma;
    }
    }
  return 0;
}

