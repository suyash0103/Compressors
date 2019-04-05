#include<iostream>
#include<fstream>
#include<unordered_map>
#include<string>
#include <ctime>

using namespace std;

void write_buffer(ofstream &o, int code, int bit_length, short int capital = 0) {
    if (bit_length == 19) {
        //cout << "19bit" << code << endl;
        if (capital){
            code = code | (3 << 17);
        }
        else code = code | (1 << 18);
        o.write((char *) &code, sizeof(code));
    }
    if (bit_length == 9) {
        //cout << "8bit" << code << endl;
        code = code | (1 << 8);
        o.write((char *) &code, sizeof(code));
    }
}

void Encode(char ipfile[], char opfile[], char dictionary[]) {
    ifstream fin(ipfile, ios::in);
    ofstream o(opfile, ios::binary);
    ifstream codefile(dictionary, ios::in);

    unordered_map<string, int> table;
    string present;
    int i = 0;
    while (!codefile.eof()) {
        codefile >> present;
        table[present] = i;
        i++;
    }

    //encoding
    string p;
    char c;
    short int capital = 0;
    int count=0;

    while (fin.get(c)) {
        if (c == 0) {
            if (table.find(p) != table.end()) {
                count++;
                write_buffer(o, table[p], 19, capital);
            } else if (p.length() >= 1) {
                if (capital)p[0] = (char)toupper(p[0]);
                for (int i = 0; i < p.length(); i++) {
                    write_buffer(o, int(p[i]), 9);
                }
            }
            capital = 0;
            p.clear();
        } else {
            if (p.length() == 0) {
                if (isalpha(c) && isupper(c)) { //if first letter is uppercase convert
                    capital = 1;
                    p += (char)tolower(c);
                } else {
                    p += c;
                }
            } else {
                p += c;
            }
        }
    }

    cout<<"number of words used from dictionary"<<count;
    fin.close();
    o.close();
    codefile.close();
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Correct usage: ./a.out tokens opfile dictionary";
        exit(0);
    }
    long long int start_s = clock();
    Encode(argv[1], argv[2], argv[3]);
    long long int stop_s = clock();
    cout << "time taken: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "ms" << endl;
}