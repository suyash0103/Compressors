#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>

using namespace std;

void Decode(char ipfile[], char opfile[], char dictionary[]) {
    ifstream fin(ipfile, ios::binary);
    ofstream o(opfile, ios::out );
    ifstream codefile(dictionary, ios::in);

    vector<string> table;
    string present;
    int i = 0;
    while (!codefile.eof()) {
        codefile >> present;
        table.push_back(present);
        i++;
    }

    int toRestore = 0;
    string s;
    while (fin.read((char *) &toRestore, 4)) {
        if(toRestore>>18){ //check 19th bit
            toRestore=toRestore ^ (1<<18);
            if(toRestore & (1<<17)){
                toRestore=toRestore ^ (1<<17);
                s=table[toRestore];
                s[0]=toupper(s[0]);
                o<<s;
            }else{
                o<<table[toRestore];
            }
            //cout<<"19 bit"<<toRestore<<endl;
        }else{
            toRestore=toRestore ^ (1<<8);
            o<<(char)toRestore;
            //cout<<"8 bit"<<toRestore<<endl;
        }
        toRestore = 0;
    }
    o.close();
    fin.close();
    codefile.close();
}

int main(int argc, char *argv[]) {
    if(argc!=4){
        cout<<"Correct usage: ./a.out ipfile opfile codebook";
        exit(0);
    }

    long long int start_s=clock();
    Decode(argv[1],argv[2],argv[3]);
    long long int stop_s=clock();
    cout << "time taken: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 <<"ms"<< endl;
    cout << "DecodedToFile:" << argv[2] << endl;

    return 0;
}