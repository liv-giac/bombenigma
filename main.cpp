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
    cout << output << endl;
    //now we pretend to forget the initial settings and message, and only know the coded message. we will have to work backwards.
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
    for (string crib : cribList){
      cribPos=output.find(crib);
      set<codePair> codePairs = readMatches(key,crib,cribPos);
      cout <<key<<endl<<crib<<endl;  
      loop = findLoop(codePairs);
      if (!loop.empty()) {
        cout << "Loop of three pairs of letters found in the crib " << crib <<": " ;
        
        cout << "(crib position "<<cribPos<< ") ";
          for (const codePair& pair : loop) {
            cout << "(" << pair.letter1 << ", " << pair.letter2 << ", "<< pair.pos <<") ";
          }
          cout << endl;
          break;
          }                 
    }
    if (loop.empty()){
      cout << "No loop of three pairs of letters found in any crib, so our Bombe cannot decode this message. " << endl;
    return 0;
    }
    //if we've found a loop, now it's time to use it to activate the actual bombe mechanism
    string prova = "POLITECNICODIMILANO";
    vector<int> rotor_indexes = {1, 2, 3};
    vector<int> rotor_pos = {1,1,2};
    vector<int> plugboard_input ={25, 8, 24, 2,1,5};
    int reflector_index = 1;
   

    Enigma *enigma = startEnigma(rotor_indexes,plugboard_input,reflector_index);
    enigma->set_pos(rotor_pos);
    cout << prova <<endl << encode(enigma, prova)<< endl;
    string trial;
     for (int i=0;i<ALPH_LEN;i++){
       for (int j=0;j<ALPH_LEN;j++){
        for (int k=0;k<ALPH_LEN;k++){
         //cout << "loop values are" << loop[0]<<loop[1]<<loop[2]<<endl;
          trial={};
          trial+='A';
          enigma->set_pos({i,j,k});
          enigma->timetravel(loop[0].pos+cribPos);
          cout << "encoding "<<trial.back()<<" into ";
          trial+=encode(enigma,trial).back();
          cout <<trial.back()<<endl;
          enigma->set_pos({i,j,k});
          enigma->timetravel(loop[1].pos+cribPos);
          cout << "encoding "<<trial.back()<<" into ";
          trial+=encode(enigma,trial).back();
          cout <<trial.back()<<endl;
          enigma->set_pos({i,j,k});
          enigma->timetravel(loop[2].pos+cribPos);
          cout << "encoding "<<trial.back()<<" into ";
          trial+=encode(enigma,trial).back();
          cout <<trial.back()<<endl;
          if (trial.back()=='A') {
            cout << "potential settings found with rotor positions "<<i<<" "<<j<<" "<<k<<"."<<endl<<"The plugboard settings would be: "<< endl;
            vector<char> loop_plugs;
            loop_plugs.push_back(loop[0].letter2);
            if (loop[0].letter2!=loop[1].letter1) loop_plugs.push_back(loop[1].letter1); else loop_plugs.push_back(loop[1].letter2);
            loop_plugs.push_back(loop[0].letter1);
            for (int m=0; m<loop_plugs.size();m++){
              cout << loop_plugs[i]<<" "<<trial[i+1]<<endl;
            }
            cout<<endl;
            break;
          } //else cout << "no solution here " << endl;
        }
      }
   }
  return 0;

}

// TO DO TOMORROW
// HAVING AS A REFERENCE THE LOOP WE HAVE FOUND, AND THE POSITIONS SO TO TIMETRAVEL CORRECTLY THE ENIGMAS

// FOR EVERY POSSIBLE ROTOR POSITION, WE NEED TO CREATE THREE ENIGMAS WITH THE CHOSEN ROTORS AND REFLECTOR, 
//WITH EMPTY PLUGBOARD AND TIME SETTING EQUAL TO THE POSITION IN THE TOTAL CODE. 
//WE "CONNECT THEM" BY CONTINUOUSLY ENCODING THE LETTER A AND IF I GET A BACK I STOP AND DECODE THE MESSAGE.
//IF IT MAKES SENSE WE WON!