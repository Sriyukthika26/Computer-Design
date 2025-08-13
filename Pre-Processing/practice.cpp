#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<unordered_set>

using namespace std;



bool ismacropresent(unordered_set<string>&macros, string macro){
    return macros.find(macro) != macros.end();
}

int main(){
    ifstream input("input.c");
    ofstream output("output.c");

    unordered_set<string>macros;

    string line;
    while(getline(input, line)){
        string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        if(trimmed.find("#define")==0){
            stringstream ss(trimmed);
            string directive, macro;
            ss >> directive >>macro;
            macros.insert(macro);
            continue;
        }
        else if(trimmed.find("#ifdef")==0){
            
        }
    }

}
