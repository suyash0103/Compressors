#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

void lzwDecode(ifstream& i,ostream& o,ifstream& codefile){
    vector<string> codebook;
    for (int i = 0; i < 256; i++) {
        codebook.push_back(std::string(1, i) = i);
    }
    string a;
    char c;
    string present;
    while (!codefile.eof()) {
        codefile.get(c);
        if (c==0) {
            codebook.push_back(present);
            present.clear();
        } else {
            present=present+c;
        }
    }

    int toRestore=0;
    while(!i.eof()){
        toRestore=0;
        i.read((char*)&toRestore,sizeof(toRestore));
        o << codebook[toRestore];
    }
}

int main (int argc, char* argv[]){
    ifstream i(argv[1],ios::binary);
    ofstream o(argv[2],ios::out|ios::trunc);
    ifstream codefile(argv[3],ios::in);
    lzwDecode(i,o,codefile);
    o.close();
    i.close();
    codefile.close();
    return 0;
}