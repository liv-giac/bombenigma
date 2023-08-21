#include "enigma.h"
#include "plug_reflect.h"
#include "rotor.h"
#include "alphabet.h"
#include <iostream>
#include <fstream>
#include <vector>

//using namespace std;

Enigma::Enigma(int argc, char** argv){
  //plugboard contacts are read, assigned to a vector for checking and then the plugboard object is created
  vector<int> plugboard_contacts;
  PlugboardConfig(argv[1], plugboard_contacts);
  plugboard = new Plug_reflect(argv[1]);
  //reflector contacts are read, assigned to a vector for checking and then the reflector object is created
  vector<int> reflector_contacts;
  ReflectorConfig(argv[2], reflector_contacts);
  reflector = new Plug_reflect(argv[2]);

  vector<int> rotor_contacts;
  vector<vector<int>> rotor_contacts_array;
  for(int i = 3; i < argc-1; i++){
    //every rotor configuration gets read and checked
    RotorConfig(argv[i], rotor_contacts);
    rotor_contacts_array.push_back(rotor_contacts);
    rotor_contacts.clear();
  }

   if(argc == 3){
    num_of_rotors = 0;
  }
   else{
     num_of_rotors = argc-4; 

   }


  RotorPositionConfig(argv[argc-1]);
  //once everything is checked we can set up the array of rotors with the data in the configuration files
  for(int i = 0; i < num_of_rotors; i++){
    Rotor rotor(argv[3+i], rotor_positions[i]);
    rotors.push_back(rotor);
  }
   //printEnigma();
}

Enigma::Enigma(vector<int> rotor_indexes, vector<int> rotor_pos, vector<int> plugboard_input, int reflector_index){
  //plugboard initialized
  plugboard = new Plug_reflect(plugboard_input);
  //reflector initialization
  string path = "reflectors/" + to_string(reflector_index) + ".rf";
  const char* path_cstr = path.c_str();
  reflector = new Plug_reflect(path_cstr);
  //set rotor positions
  rotor_positions=rotor_pos;
  num_of_rotors = 3;
  for(int i = 0; i < 3; i++){
    path = "rotors/" + to_string(rotor_indexes[i]) + ".rot";
  const char* path_cstr = path.c_str();
    Rotor rotor(path_cstr, rotor_positions[i]);
    rotors.push_back(rotor);
    }
     //printEnigma();
  }

bool Enigma::PlugboardCheck(const char* path, fstream& in_stream, int& index_num){
  in_stream >> ws;
  if(in_stream.peek() == EOF){
    return false;
  }
  in_stream >> index_num;
  if(in_stream.fail()){
    cout << "Incompatible character in plugboard configuration file " << path << endl;
    in_stream.close();
  }
  if(!RangeCheck(index_num)){
    cout << "The plugboard configuration file " << path << " contains a number not in 0-25 range" << endl;
    in_stream.close();
  }
  return true;
}

void Enigma::PlugboardConfig(const char* path, vector<int>& contacts){
  int even_index_num, odd_index_num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cout << "Error with the plugboard configuration file " << path << endl;
    in_stream.close();
  }
  while(!in_stream.eof()){
    if(!PlugboardCheck(path, in_stream, even_index_num)){
      break;
    }
    if(!PlugboardCheck(path, in_stream, odd_index_num)){
      cout << "Wrong number of parameters in the plugboard configuration file " << path << endl;
      in_stream.close();
    }
    contacts.push_back(even_index_num);
    if(AppearedBefore(contacts, even_index_num, counter) != -1){
      in_stream.close();
      cout << "Invalid plugboard configuration!" << endl;
    }
    contacts.push_back(odd_index_num);
    counter++;

    if(AppearedBefore(contacts, odd_index_num, counter) != -1){
      in_stream.close();
      cout << "Invalid plugboard configuration!" << endl;
    }
    counter++;
  }
  in_stream.close();
}

void Enigma::ReflectorConfig(const char* path, vector<int>& contacts){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cout<< "Error with the reflector configuration file " << path << endl;
    in_stream.close();
  }

  while(!in_stream.eof()){
    in_stream >> ws;
    if(in_stream.peek() == EOF){
      break;
    }
    in_stream >> num;
    if(in_stream.fail()){
      cout << "Invalid character in reflector configuration file" << path << endl;
      in_stream.close();
    }
    if(!RangeCheck(num)){
      cout << "The reflector configuration file " << path <<" contains a number that is not between 0 and 25" << endl;
      in_stream.close();

    }
    contacts.push_back(num);
    if(counter < ALPH_LEN && AppearedBefore(contacts, num, counter) != -1){
      in_stream.close();
       cout << "Invalid reflector mapping!" <<endl;}
    counter++;

  }
  in_stream.close();

  if(counter%2!=0) cout<< "Wrong number of parameters in reflector configuration file " << path << endl;
  if(counter != ALPH_LEN) cerr << "Incomplete mappings in reflector configuration file: " << path << endl;

}
void Enigma::RotorConfig(const char* path, vector<int>& contacts){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cout << "Error with the rotor configuration file " << path << endl;
    in_stream.close();
  }
  while(!in_stream.eof()){
    in_stream >> ws;
    if(in_stream.peek() == EOF) break;
    in_stream >> num;
    if(in_stream.fail()){
      cout << "Invalid character in rotor configuration file "  << path << endl;
      in_stream.close();
    }
    if(!RangeCheck(num)){
      cout << "The file " << path << " contains a number that isn't in the [0-25] range" << endl;
      in_stream.close();
    }
    contacts.push_back(num);
    if(counter < ALPH_LEN-1 && AppearedBefore(contacts, num, counter) != -1){
      in_stream.close();
      cout << "The mapping is invalid/incomplete (repeated values)" << endl;
    }
    counter++;
  }
  in_stream.close();
  if(counter < ALPH_LEN)cout << "The mapping is invalid/incomplete (not enough values)" << endl;
}

void Enigma::RotorPositionConfig(const char* path){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cout << "Error with the rotor configuration file " << path << endl;
    in_stream.close();
  }
  while(!in_stream.eof()){
    in_stream >> ws;
    if(in_stream.peek() == EOF) break;
     in_stream >> num;
    if(in_stream.fail()){
      cout << "Incompatible value in the rotor configuration file" << path  << endl;
      in_stream.close();
    }
    if(!RangeCheck(num)){
      cout << "The file " << path << " contains a number that is not between 0 and 25 CHANGE " << endl;
      in_stream.close();
    }
    counter++;
    rotor_positions.push_back(num);
  }

  if(counter - num_of_rotors < 0){
    cout << "Not enough starting positions provided in file " << path << endl;
    in_stream.close();
  }
  in_stream.close();
}

bool Enigma::RangeCheck(int num){
  return (num < ALPH_LEN && num >= 0);
}

int Enigma::AppearedBefore(vector<int> contacts, int num, int position){
  for(int i = 0; i < position; i++ ){
    if(contacts[i] == num){
      cout << "Value " << num << " is already mapped in position " << i << endl;
      return i;
    }
  }
  return -1;
}

void Enigma::timetravel(int time){
  for (int j=0; j<time;j++){
  if(num_of_rotors > 0){
    rotors[num_of_rotors-1].rotate();
    for(int i = num_of_rotors; i > 0; i--){
      if(rotors[i-1].isItNotch() && rotors[i-1].getPreviousPosition() != rotors[i-1].getCurrentPosition()){
        if(i-1 > 0){
          rotors[i-2].rotate();
         } 
      } 
      }
  }
  }
  vector<int> new_pos;
  new_pos.push_back(rotors[0].getCurrentPosition());
  new_pos.push_back(rotors[1].getCurrentPosition());
  new_pos.push_back(rotors[2].getCurrentPosition());
  rotor_positions=new_pos;
}

void Enigma::encryptMessage(char& letter){
  int curr_index = letter - 'A';

  curr_index = plugboard->map(curr_index);


  if(num_of_rotors> 0){
    rotors[num_of_rotors-1].rotate();
  }

  if(num_of_rotors > 0){
    for(int i = num_of_rotors; i > 0; i--){
      curr_index = rotors[i-1].shiftDown(curr_index);
      curr_index = rotors[i-1].mapForward(curr_index);
      curr_index = rotors[i-1].shiftUp(curr_index);
      if(rotors[i-1].isItNotch() && rotors[i-1].getPreviousPosition() != rotors[i-1].getCurrentPosition()){
        if(i-1 > 0){
          rotors[i-2].rotate();
         } 
      } 

      }
  }

  curr_index = reflector->map(curr_index); 

    if(num_of_rotors > 0){
    for(int i = 0; i < num_of_rotors; i++){
      curr_index = rotors[i].shiftDown(curr_index);
      curr_index = rotors[i].mapBackward(curr_index);
      curr_index = rotors[i].shiftUp(curr_index);

     }
  }

  curr_index = plugboard->map(curr_index);

  letter = curr_index + 'A';
}

void Enigma::printEnigma(){
  cout << "the plugboard settings are "<< endl;
  vector<int> pair1 = plugboard->getpair1();
  vector<int> pair2 = plugboard->getpair2();
  for (int i=0; i<pair1.size();i++){
    cout << pair1[i]<<" "<<pair2[i]<<endl;
  }
  cout<<endl;

  cout << "the reflector settings are "<< endl;
  pair1 = reflector->getpair1();
  pair2 = reflector->getpair2();
  for (int i=0; i<pair1.size();i++){
    cout << pair1[i]<<" "<<pair2[i]<<endl;
  }
  cout<<endl;
  for (int i=0; i<num_of_rotors;i++){
  cout << "the rotor "<< i+1 <<" settings are "<< endl;
  vector<int> info = rotors[i].getNotchAndPos();
  cout << "notch "<<info[0]<<" and initial position "<< info[1] <<  endl;
  vector<int> contacts = rotors[i].getContacts();
  for (int i=0; i<contacts.size();i++){
    cout << contacts[i]<<endl;
  }
  cout<<endl;

  }
}
//encryption that prints out the decoding, used it to debug
// void Enigma::encryptMessage(char& letter){
//   int curr_index = letter - 'A';
//   cout << "mapping " << curr_index << " into ";
//   curr_index = plugboard->map(curr_index);
//   cout << curr_index << " through the plugboard" << endl;
//       cout << "mapping " << curr_index << " into ";
//   if(num_of_rotors> 0){
//     rotors[num_of_rotors-1].rotate();
//   }

//   if(num_of_rotors > 0){
//     for(int i = num_of_rotors; i > 0; i--){
//       curr_index = rotors[i-1].shiftDown(curr_index);
//       curr_index = rotors[i-1].mapForward(curr_index);
//       curr_index = rotors[i-1].shiftUp(curr_index);
//       if(rotors[i-1].isItNotch() && rotors[i-1].getPreviousPosition() != rotors[i-1].getCurrentPosition()){
//         if(i-1 > 0){
//           rotors[i-2].rotate();
//          } 
//       } 
//       cout << curr_index << " by rotor number "<< i <<" , then into ";
//       }
//   }
//   cout << "the reflector."<< endl << "mapping " << curr_index << " into ";
//   curr_index = reflector->map(curr_index); 
//   cout << curr_index << " through the reflector" << endl;
//   cout << "mapping " << curr_index << " into ";
//     if(num_of_rotors > 0){
//     for(int i = 0; i < num_of_rotors; i++){
//       curr_index = rotors[i].shiftDown(curr_index);
//       curr_index = rotors[i].mapBackward(curr_index);
//       curr_index = rotors[i].shiftUp(curr_index);
//       cout << curr_index << " by rotor number "<< i <<" , then into ";
//      }
//   }
//    cout << "the plugboard."<< endl << "mapping " << curr_index << " into ";
//   curr_index = plugboard->map(curr_index);
//   cout << curr_index << " through the plugboard" << endl;
//   letter = curr_index + 'A';
// }
 