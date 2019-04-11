#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <chrono>

using namespace std::chrono;
using namespace std;

void Decode(char ipfile[], char opfile[], char dictionary[]) {
    ifstream fin(ipfile, ios::binary);
    ofstream o(opfile, ios::binary );
    ifstream codefile(dictionary, ios::in);

    vector<string> table;
    string present;
    int i = 0;
    while (!codefile.eof()) {
        codefile >> present;
        table.push_back(present);
        i++;
    }

    string s;
    char c;
    uint64_t code;
    while(1){
        if (!fin.read(&c, sizeof(c))){break;}
//        cout<<"char:"<<c<<endl;
        if(c==-125 || c==-124){
            code=0;
            if (!fin.read((char *) &code, 2)) break;

            if(code==0){break;cout<<"break1\n";}
            if(c==-124){
                s=table[code];
                s[0]=toupper(s[0]);
                o<<s;
                //cout<<"\n"<<s<<" "<<code<<"\n";
            }else{
                s=table[code];
                o<<s;
                //cout<<"\n"<<s<<" "<<code<<"\n";
            }
        }else{
            if(c==0){break;cout<<"break2\n";}
            o<<(char)c;
            //cout<<(char)code<<":"<<code<<endl;
        }
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

    auto start = high_resolution_clock::now();
    Decode(argv[1],argv[2],argv[3]);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;
    return 0;
}