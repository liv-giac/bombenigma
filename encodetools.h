#ifndef ENCODETOOLS_H
#define ENCODETOOLS_H

#include "enigma.h"
#include "alphabet.h"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

string encodeFullMessage(int argc, char** argv){
//first part is to create the encoded message with "today's enigma settings"
//i need to provide at least a plugboard and reflector file, and if i am providing rotors i need to also provide the position configuration file
  if(argc < 3 || argc == 4) cout << "Wrong number of arguments" <<endl;
  char letter;
  Enigma *enigma = nullptr;
  enigma = new Enigma(argc, argv);
  string output = "";
  while(cin >> letter){
    if(cin.fail()){
      break;
    }
    if(letter - 'A' < 0 || ALPH_LEN -1 < letter - 'A'){
      cout << letter << " is not a valid input character (input characters must be upper case letters A-Z)!" << endl;
      delete enigma;
      return 0;
    }
    enigma->encryptMessage(letter);
    output += (char) letter;
  }
  delete enigma;
  return output;
}

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

struct codePair{
    char letter1;
    char letter2;
    int pos;

    
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

set<codePair>  readMatches(string key, string crib){
  set<codePair> codePairs;
  int len = key.size();
  for (int i=0; i<len;i++){
    if (key[i]<=crib[i]) codePairs.insert({key[i], crib[i],i});
    else codePairs.insert({crib[i], key [i],i});
  }
  return codePairs;
}

vector<codePair> findLoop(set<codePair> pairs){
  vector<codePair> loop;
  char temp;
  for (codePair pair1 : pairs){
    if (!loop.empty()) break;
    temp=pair1.letter2;
    for (codePair pair2 : pairs){
      if (1-(pair1==pair2)){
        if (pair2.letter1==temp) {
          temp=pair2.letter2;}
        else if (pair2.letter2==temp) temp=pair2.letter1;
        else continue;
      for (codePair pair3 : pairs){
        if (!(pair3==pair2)&&!(pair3==pair1)){
          if (pair3.letter1==temp) {
            temp=pair3.letter2;
            if (pair1.letter1==temp){
              loop.push_back(pair1);
              loop.push_back(pair2);
              loop.push_back(pair3);
            }
          }
          else if (pair3.letter2==temp) {
            temp=pair3.letter1;
            if (pair1.letter1==temp){
              loop.push_back(pair1);
              loop.push_back(pair2);
              loop.push_back(pair3);
            }
          }
      }
      }
    }
    }
  }
return loop;
}

// vector<codePair> findLoop(const set<codePair>& codePairSet, codePair current, codePair initial, int length, vector<codePair>& path) {
//     if (length == 3 && current == initial) {
//         // Found a loop of three; return it
//         return path;
//     }

//     // Mark the current codePair as visited
//     path.push_back(current);
//     cout << endl << "adding "<< current.letter1 << " " << current.letter2 << " to the path, current length is " << length << endl;

//     for (const codePair& next : codePairSet) {

//         // Check if the next codePair is adjacent and not visited
//         cout << "considering pair "<<next.letter1<<" "<<next.letter2<<" as next"<<endl;
//         if ((current.letter2 == next.letter1 || current.letter2 == next.letter2 ||current.letter1 == next.letter1 ||current.letter1 == next.letter2) && (find(path.begin(), path.end(), next) == path.end())) {
//             vector<codePair> loop = findLoop(codePairSet, next, initial, length + 1, path);
//             if (!loop.empty()) {
//                 return loop; // Found a loop, return it
//             }
//         }
//     }

    // Unmark the current codePair (backtrack)
//     path.pop_back();

//     return {}; // No loop found
// }

// vector<codePair> findLoop(set<codePair> pairs){
//   vector<codePair> loop;
//   char temp;
//   for (codePair pair1 : pairs){
//     if (!loop.empty()) break;
//     temp=pair1.letter2;
//     cout <<"searching for letter "<< temp << " from pair "<< pair1.letter1 << temp<<endl;
//     cout <<"starting second for"<<endl;
//     for (codePair pair2 : pairs){
//       cout <<"picked out pair "<< pair2.letter1 << pair2.letter2<<endl;
//       if (1-(pair1==pair2)){
//         cout <<"searching for letter "<< temp << " in pair "<< pair2.letter1 << pair2.letter2<<endl;
//         if (pair2.letter1==temp) {
//           temp=pair2.letter2;}
//         else if (pair2.letter2==temp) temp=pair2.letter1;
//         else continue;
//       cout <<"starting third for"<<endl;
//       for (codePair pair3 : pairs){
//         if (!(pair3==pair2)&&!(pair3==pair1)){
//           cout <<"searching for letter "<< temp << " in pair "<< pair3.letter1 << pair3.letter2<<endl;
//           if (pair3.letter1==temp) {
//             temp=pair3.letter2;
//             if (pair1.letter1==temp){
//               loop.push_back(pair1);
//               loop.push_back(pair2);
//               loop.push_back(pair3);
              
//             }
//           }
//           else if (pair3.letter2==temp) {
//             temp=pair3.letter1;
//             if (pair1.letter1==temp){
//               loop.push_back(pair1);
//               loop.push_back(pair2);
//               loop.push_back(pair3);
              
//             }
//           }
//       }
//       }
//     }
//     }
//   }
// return loop;
// }



#endif