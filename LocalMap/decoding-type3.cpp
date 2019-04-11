#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <chrono>

using namespace std::chrono;
using namespace std;

void Decode(char ipfile[], char ipfile1[], char opfile[], char dictionary[]) {
    ifstream fin(ipfile, ios::binary);
    ifstream fin2(ipfile1,ios::binary);
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
    uint64_t buff = 0,code;
    if (!fin.read((char *) &buff, sizeof(buff))) return;
    while(1){
        //read first char and find if its 18 bit or 8 bit encoding
        if (!fin2.read(&c, sizeof(c))){break;cout<<"readbreak\n";}
//        cout<<"char:"<<c<<endl;
        if(c==-125){
            code=0;
            if (buff_read(&buff, &code,18, 0) == 0) {
                if (!fin.read((char *) &buff, sizeof(buff))) break;
                buff_read(&buff, &code,18, 0);
            }
            if(code==0){break;cout<<"break1\n";}
            if(code & (1<<17)){
                //cout<<code<<":"<<(code ^ (1<<17))<<endl;
                code=code ^ (1<<17);
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
    fin2.close();
    codefile.close();
}

int main(int argc, char *argv[]) {
    if(argc!=5){
        cout<<"Correct usage: ./a.out ipfile1 ipfile2 opfile codebook";
        exit(0);
    }

    auto start = high_resolution_clock::now();
    Decode(argv[1],argv[2],argv[3],argv[4]);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;
    return 0;
}