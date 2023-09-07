#ifndef ENCODETOOLS_H
#define ENCODETOOLS_H

#include "enigma.hpp"
#include "alphabet.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

vector<int> readplug(const string& input) {
    vector<int> output;
    string temp;

    for (char c : input) {
        if (c == ' ') {
            if (!temp.empty()) {
                output.push_back(stoi(temp));
                temp.clear();
            }
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) {
        output.push_back(stoi(temp));
    }
    return output;
}

//Encoding a series of full messages with "hidden settings" that we only have in the makefile
vector<string> encodeFullMessages(int argc, char** argv){

  if(argc < 3 || argc == 4) cout << "Wrong number of arguments" <<endl;
  Enigma *enigma = nullptr;
  enigma = new Enigma(argc, argv);
  vector<string> output;
  string input_line,output_line;
  
   while (std::getline(std::cin, input_line) && !input_line.empty()) {//We encode all the messages in the input file
        for (char letter : input_line){
        enigma->encryptMessage(letter);
        output_line += (char) letter;
        }
        output.push_back(output_line);
        output_line={};
    }
  delete enigma;
  return output;
}
//This function takes a string and a correctly setup enigma and uses the machine to encode the string
string encode(Enigma *enigma, string input){
  
  string output = "";
  for(char letter : input){
    enigma->encryptMessage(letter);
    output += letter;
  }
  return output;
}

//Creates an enigma with the correct settings and returns address to the enigma machine
Enigma* startEnigma(vector<int> rotor_indexes, vector<int> plugboard_input, int reflector_index){
  Enigma *enigma = nullptr;
  enigma = new Enigma(rotor_indexes, {0,0,0}, plugboard_input, reflector_index);
  return enigma;
}
//Reads the Key from input file
string getKeyFromFile(){
   ifstream file("key.txt"); 
    if (!file.is_open()) {
        cout << "Error: Could not open the file." << endl;
        return "";
    }
    std::string key;
    getline(file, key);
    file.close();
    return key;
}
//Gets the cribs looking for all the places where the key could be in the enrypted message
vector<string> getCribs(string output, string key){
   int len = key.size();
   int len2 = output.size();
   int check=0;
   string crib="";
   vector<string> cribs;
   for (int i=0; i<len2-len+1; i++){
    for (int j=0;j<len; j++){
      if (output[i+j]==key[j]) check = 1;
      crib += output[i+j];
    }
    if (check == 0) cribs.push_back(crib);
    crib = "";
    check = 0;
   }
   return cribs;
}

//A structure that allows us to save the couples of letters and where they are in the crib-output position
struct codePair{
    char letter1;
    char letter2;
    int pos;
    //this operator permits a sorting
    bool operator<(const codePair& other) const {
        if (letter1 != other.letter1) {
            return letter1 < other.letter1;
        } else if (letter2 != other.letter2) {
            return letter2 < other.letter2;
        } else {
            return pos < other.pos;
        }
    }
    bool operator==(const codePair& other) const {
    return letter1 == other.letter1 && letter2 == other.letter2 && pos == other.pos;
}
};



//This function extracts the pairs and their positions from crib and key
set<codePair>  readMatches(string key, string crib, int cribPos){
  set<codePair> codePairs;
  int len = key.size();
  for (int i=0; i<len;i++){
    if (key[i]<=crib[i]) codePairs.insert({key[i], crib[i],i+cribPos});
    else codePairs.insert({crib[i], key [i],i+cribPos});
  }
  return codePairs;
}

//This function finds a loop in the matches in the correspondence between crib and key
vector<codePair> findLoop(set<codePair> pairs){
  vector<codePair> loop;
  char temp;
  for (codePair pair1 : pairs){
    if (!loop.empty()) break;
    for (codePair pair2 : pairs){
      temp=pair1.letter2;
      if (!(pair1==pair2)){
        if (pair2.letter1==temp) temp=pair2.letter2;
        else if (pair2.letter2==temp) temp=pair2.letter1;
        else continue;
      for (codePair pair3 : pairs){
        if (!(pair3==pair2)&&!(pair3==pair1)){
          if (pair3.letter1==temp) {
            if (pair1.letter1==pair3.letter2){
              loop.push_back(pair1);
              loop.push_back(pair2);
              loop.push_back(pair3);
              return loop;
            }
          }
          else if (pair3.letter2==temp) {
            if (pair1.letter1==pair3.letter1){
              loop.push_back(pair1);
              loop.push_back(pair2);
              loop.push_back(pair3);
              return loop;
            }
          }
        }
      }
      }
    }
  }
return loop;
}



#endif