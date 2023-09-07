#include "enigma.hpp"
#include "alphabet.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include "encodetools.hpp"
#include <omp.h>
#include <chrono>
#include <algorithm>
using namespace std;



int main(int argc, char** argv){
   auto start_time = chrono::high_resolution_clock::now();

  //first we encode the messages knowing "today's settings" and the original messages
    vector<string> outputs = encodeFullMessages(argc, argv);
    //we will only need one message
    string output=outputs.front();
    vector<vector<int>> plugboards={};
    vector<int> plugboard;
    string line;
    ifstream pluglist("pluglist.txt");
    while (getline(pluglist, line)) {
        plugboard=readplug(line);
        plugboards.push_back(plugboard);
    }
    pluglist.close();

    //now we pretend to forget the initial settings and messages, and only know the coded messages. we will have to work backwards.
    //we read the key from file
    string key= getKeyFromFile(); 
    //we find the possible cribs
    vector<string>  cribList = getCribs(output, key);
    reverse(cribList.begin(), cribList.end()); //the string we're looking for was always towards the end so we start at the end to reduce potential computational times
    int n_cribs=cribList.size();
    int cribPos;
    int threads=min(n_cribs,8);
   
    #pragma omp parallel for num_threads(threads) shared(cribList) private(cribPos)
    for (string crib : cribList){//we try for all cribs
      cribPos=output.find(crib);
      
    //we assume to know which reflector and which rotors are inserted
    vector<int> rotor_indexes = {1, 2, 3};
    int reflector_index = 1;
    
    //we iterate through different possible plugboard settings
     #pragma omp parallel for num_threads(threads) private(plugboard)
     for (vector<int> plugboard:plugboards) {
        Enigma *enigma = startEnigma(rotor_indexes,plugboard,reflector_index);
        string decoded;
        for (int i=0;i<ALPH_LEN;i++){
          for (int j=0;j<ALPH_LEN;j++){
            for (int k=0;k<ALPH_LEN;k++){
              enigma->set_pos({i,j,k});
              enigma->timetravel(cribPos);//the timetravel function sets the rotor to its position after x encodings
              decoded=encode(enigma,crib);
              if (decoded==key){
                 auto end_time = chrono::high_resolution_clock::now();
                cout<<"Found solution at rotor positions "<<i<<" "<<j<<" "<<k<<"."<<endl<<"with plugboard settings ";
                for (int a : plugboard){
                  char letter = 'A' + a;
                  cout << letter << " ";
                }
                cout<< endl;
                enigma->set_pos({i,j,k});//we print all the decoded messages
                cout<<"Decoded messages: "<<endl;
                for (string code :outputs){
                  cout<<encode(enigma,code)<<endl;
                }
                delete enigma;
                //we print elapsed time
                auto elapsed_time = chrono::duration_cast<chrono::seconds>(end_time - start_time);
                cout << "Time taken: " << elapsed_time.count() << " seconds" << endl;
                exit(0);
              }
            } 
          }
        }
     
      delete enigma;
      }
      }
    
  
  return 0;
}

