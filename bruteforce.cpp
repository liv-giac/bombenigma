#include "enigma.hpp"
#include "alphabet.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include "encodetools.hpp"
#include <omp.h>
using namespace std;



int main(int argc, char** argv){
  //first we encode the messages knowing "today's settings" and the original messages
    vector<string> outputs = encodeFullMessages(argc, argv);
    //we will only need one message
    string output=outputs.front();
    cout << "The message we try to decode is " <<output << endl;
    //now we pretend to forget the initial settings and messages, and only know the coded messages. we will have to work backwards.
    //we read the key from file
    string key= getKeyFromFile(); 
    //we find the possible cribs
    vector<string>  cribList = getCribs(output, key);
    reverse(cribList.begin(), cribList.end());
    int n_cribs=cribList.size();
    cout << "number of cribs found: " <<n_cribs<<endl;
    int cribPos;
    int threads=min(n_cribs,8);
   
    #pragma omp parallel for num thread(threads) shared(cribList) private(cribPos,pluglist)
    for (string crib : cribList){//we try for all cribs
      cribPos=output.find(crib);
      ifstream pluglist("pluglist.txt");
 
    //we assume to know which reflector and which rotors are inserted
    vector<int> rotor_indexes = {1, 2, 3};
    int reflector_index = 1;
    
    string line;
    
     while (getline(pluglist, line)) {
        vector<int> plugboard=readplug(line);
        Enigma *enigma = startEnigma(rotor_indexes,plugboard,reflector_index);
        string decoded;
        for (int i=0;i<ALPH_LEN;i++){
          for (int j=0;j<ALPH_LEN;j++){
            for (int k=0;k<ALPH_LEN;k++){
              enigma->set_pos({i,j,k});
              enigma->timetravel(cribPos);//the timetravel function sets the rotor to its position after x encodings
              decoded=encode(enigma,crib);
              if (decoded==key){
                cout<<"Found solution at rotor positions "<<i<<" "<<j<<" "<<k<<"."<<endl<<"with plugboard settings ";
                for (int a : plugboard){
                  char letter = 'A' + a;
                  cout << letter << " ";
                }
                cout<< endl;
                delete enigma;
                pluglist.close();
                return 0;
              }
            } 
          }
        }
     
      delete enigma;
      }
      pluglist.close();
      }
    
  
  return 0;
}


