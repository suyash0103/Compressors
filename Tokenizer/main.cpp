#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include <limits.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Correct usage: ./a.out ipfile opfile";
        exit(0);
    }

    ifstream i(argv[1], ios::binary);
    ofstream o(argv[2], ios::binary);
    char c, delimiter;
    delimiter = 0;
    int tokens=0,words=0;

    string p;
    p.clear();
    while (i.read(&c,sizeof(c))) {
        //cout<<int(c);
        if (isalpha(c)) {
            p += c;
        } else {
            if (p.length() != 0) {
                o << p;
                o.write((char *) &delimiter, sizeof(delimiter));
                p.clear();
                words+=1;
                tokens+=1;
            }
            o.write((char *) &c, sizeof(c));
            o.write((char *) &delimiter, sizeof(delimiter));
            tokens+=1;
        }
    }
    if(p.length()!=0){
        o << p;o.write((char *) &delimiter, sizeof(delimiter));
    }
    i.close();
    o.close();
    cout<<"Tokens: "<<tokens<<" Words: "<<words<<endl;
    return 0;
}
