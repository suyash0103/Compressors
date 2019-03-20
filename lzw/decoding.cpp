#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

void lzwDecode(char ipfile[], char opfile[], char file[]) {
    ifstream i(ipfile, ios::binary);
    ofstream o(opfile, ios::out | ios::trunc);
    ifstream codefile(file, ios::in);

    vector <string> codebook;
    codebook.reserve(4096);
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
            present+=c;
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
    o.close();
    i.close();
    codefile.close();
}

int main(int argc, char *argv[]) {
    if(argc!=4){
        cout<<"Correct usage: ./a.out ipfile opfile codebook";
        exit(0);
    }

    long long int start_s=clock();
    lzwDecode(argv[1],argv[2],argv[3]);
    long long int stop_s=clock();
    cout << "time taken: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 <<"ms"<< endl;
    cout << "DecodedToFile:" << argv[2] << endl;

    return 0;
}