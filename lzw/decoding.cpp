#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

void lzwDecode(ifstream &i, ostream &o, ifstream &codefile) {
    vector <string> codebook;
    for (int i = 0; i < 256; i++) {
        codebook.push_back(std::string(1, i) = i);
    }
    string a;
    char c;
    string present;
    while (!codefile.eof()) {
        codefile.get(c);
        if (c == 0) {
            codebook.push_back(present);
            present.clear();
        } else {
            present = present + c;
        }
    }

    int toRestore = 0;
    while (!i.eof()) {
        toRestore = 0;
        i.read((char *) &toRestore, 3);
        if (toRestore >> 12)
            o << codebook[toRestore >> 12];
        int hex = 0xFFF;
        toRestore = (toRestore & hex);
        if (toRestore)
            o << codebook[toRestore];
    }
}

int main(int argc, char *argv[]) {
    if(argc!=4){
        cout<<"Correct usage: ./a.out ipfile opfile codebook";
        exit(0);
    }
    ifstream i(argv[1], ios::binary);
    ofstream o(argv[2], ios::out | ios::trunc);
    ifstream codefile(argv[3], ios::in);
    lzwDecode(i, o, codefile);
    cout << "DecodedToFile:" << argv[2] << endl;
    o.close();
    i.close();
    codefile.close();
    return 0;
}