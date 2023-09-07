
#include "alphabet.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct plug{
    int first;
    int second;
    //this operator permits a sorting
    bool operator<(const plug& other) const {
        if (first != other.first) {
            return first < other.first;
        } else if (second != other.second) {
            return first < other.second;
        } 
    }
    bool operator==(const plug& other) const {
    return first == other.first&& second == other.second;
}
};

bool shareletter(plug plug1, plug plug2){
    if (plug1.first!=plug2.first && plug1.first!=plug2.second && plug1.second!=plug2.first && plug1.second!=plug2.second)
        return 0;
        else return 1;
}


int main(){
     
    vector<plug> plugs;
    plug temp;
    ofstream outFile("pluglist.txt"); // Open the output file
    
    for (int i=0;i<ALPH_LEN;i++){
       for (int j=0;j<i;j++){
        temp.first=i;
        temp.second=j;
        plugs.push_back(temp);
       }}
    vector<vector<plug>> plugboards;
    vector<plug> plugboard;
    int len=plugs.size();
    for (int i=0;i<len;i++){
       for (int j=0;j<i;j++){ 
        if (!shareletter(plugs[i],plugs[j])){
            for (int k=0;k<j;k++){
                if (!shareletter(plugs[k],plugs[j])&&!shareletter(plugs[k],plugs[i])){
                    plugboard={};
                    plugboard.push_back(plugs[k]);
                    plugboard.push_back(plugs[j]);
                    plugboard.push_back(plugs[i]);
                    plugboards.push_back(plugboard);
                    outFile << plugboard[0].first<<" "<<plugboard[0].second<<" "<<plugboard[1].first<<" "<<plugboard[1].second<<" "<<plugboard[2].first<<" "<<plugboard[2].second<<endl;
                }
            }
        }
       }
    }

    




    outFile.close(); // Close the output file

    return 0;}