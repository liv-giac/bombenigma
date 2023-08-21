#include "enigma.h"
#include "alphabet.h"
#include <iostream>
#include <cstring>
#include <string>
#include "encodetools.h"

using namespace std;

int main(int argc, char** argv){
  //first we encode the message knowing "today's settings" and the original message
    //string output = encodeFullMessage(argc, argv);
    string output = "WJSIOAWOTHFULAGRPDNTAZXBBOEBIMGAFRWOR";
    //now we pretend to forget the initial settings and message, and only know the coded message. we will have to work backwards.
    //first we read the key from file
    string key= getKeyFromFile();
    //then we need to find in the message a crib: a string of the same length of the key where there are no letters that correspond to itself
    vector<string>  cribList = getCribs(output, key);
    int n_cribs=cribList.size();
    if (n_cribs==0) {cout << "There are no cribs, so our Bombe cannot decode this message."; return 0; }
    //if we have some suitable cribs, we can go on to search for a loop in the coupling of letters
    vector<codePair> loop;
    int cribPos;
    //we only need one loop so as soon as we find it we can stop
    for (string crib : cribList){
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
          break;
          }                 
    }
    if (loop.empty()){
      cout << "No loop of three pairs of letters found in any crib, so our Bombe cannot decode this message. " << endl;
    return 0;
    }
    //if we've found a loop, now it's time to use it to activate the actual bombe mechanism
    string prova = "LPOLITECNICODIMILANOCHIUDEPERFERIE";
    vector<int> rotor_indexes = {1, 2, 3};
    vector<int> rotor_pos = {3, 7, 5};
    vector<int> plugboard_input ={25, 8, 24, 2};
    int reflector_index = 1;
    int t=0;
    prova = encode(rotor_indexes, rotor_pos, plugboard_input, reflector_index, output, t);
    cout << prova <<endl << output<< endl;
  
    vector<string> trial={"F","",""};
    //       trial[1]=encode(rotor_indexes,{3, 7, 5},{},reflector_index,trial[0],loop[0].pos+cribPos);
    //       cout << "just encoded "<<trial[0]<<" into "<< trial[1];
    //       trial[2]=encode(rotor_indexes,{3, 7, 5},{},reflector_index,trial[1],loop[1].pos+cribPos);
    //       cout << "just encoded "<<trial[1]<<" into "<< trial[2];
    //       trial[3]=encode(rotor_indexes,{3, 7, 5},{},reflector_index,trial[2],loop[2].pos+cribPos);
    //       cout << "just encoded "<<trial[2]<<" into "<< trial[3];
    //       if (trial[0]==trial[3]) {
    //         cout << "potential settings found with rotor positions "<<3<<" "<<7<<" "<<4<<"."<<endl<<"The plugboard settings would be: "<< endl;
    //         vector<char> loop_plugs;
    //         loop_plugs.push_back(loop[0].letter2);
    //         if (loop[0].letter2!=loop[1].letter1) loop_plugs.push_back(loop[1].letter1); else loop_plugs.push_back(loop[1].letter2);
    //         loop_plugs.push_back(loop[0].letter1);
    //         for (int m=0; m<loop_plugs.size();m++){
    //           cout << loop_plugs[m]<<" "<<trial[m+1]<<endl;
    //         }
    //         cout<<endl;
    //       }
    for (int i=0;i<ALPH_LEN;i++){
      for (int j=0;j<ALPH_LEN;j++){
        for (int k=0;k<ALPH_LEN;k++){

          trial={"A","",""};
          trial[1]=encode(rotor_indexes,{i,j,k},{},reflector_index,trial[0],loop[0].pos+cribPos);
          trial[2]=encode(rotor_indexes,{i,j,k},{},reflector_index,trial[1],loop[1].pos+cribPos);
          trial[3]=encode(rotor_indexes,{i,j,k},{},reflector_index,trial[2],loop[2].pos+cribPos);
          if (trial[0]==trial[3]) {
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
          } else cout << "no solution here " << endl;
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