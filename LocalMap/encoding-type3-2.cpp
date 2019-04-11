#include<iostream>
#include<fstream>
#include<unordered_map>
#include<string>
#include<ctime>
#include<chrono>

using namespace std::chrono;
using namespace std;

void prepare_buffer(uint64_t &buff, ofstream &o, ofstream &o2, int code_32, int bit_length, short int capital = 0) {
    char delimitersmall=-125;
    char delimitercapital=-124;
    if (bit_length == 16) {
        if (capital) {
            o.write(&delimitercapital, sizeof(delimitercapital));
        }else{
            o.write(&delimitersmall,sizeof(delimitersmall));
        }
        //writing to file
        o2.write((char*)&code_32,2);
    }
    if (bit_length == 8) {
        o.write((char*)&code_32,1);
    }
}

void Encode(char ipfile[], char opfile[], char opfile2[], char dictionary[]) {
    ifstream fin(ipfile, ios::binary);
    ofstream o(opfile, ios::binary);
    ofstream o2(opfile2, ios::binary);
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
    unsigned char temp;
    short int capital = 0;
    int count = 0;
    uint64_t buff = 0;

    while (fin.get(c)) {
        if (c == 0) {
            if (table.find(p) != table.end()) {
                count++;
                prepare_buffer(buff, o, o2, table[p], 16, capital);
                //cout<<" "<<p;
            } else if (p.length() >= 1) {
                if (capital)p[0] = (char) toupper(p[0]);
                for (int i = 0; i < p.length(); i++) {
                    temp=int(p[i]);
                    prepare_buffer(buff, o, o2, temp, 8);
                    //cout<<p[i]<<" "<<int(p[i])<<endl;
                }
            }
            capital = 0;
            p.clear();
        }
        else
        {
            if (p.length() == 0) {
                if (isalpha(c) && isupper(c)) { //if first letter is uppercase convert
                    capital = 1;
                    p += (char) tolower(c);
                } else {
                    p += c;
                }
            } else {
                p += c;
            }
        }
    }

    cout << "number of words used from dictionary " << count << "\n";
    fin.close();
    o.close();
    o2.close();
    codefile.close();
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cout << "Correct usage: ./a.out tokens opfile1 opfile2 dictionary";
        exit(0);
    }
    auto start = high_resolution_clock::now();
    Encode(argv[1], argv[2], argv[3], argv[4]);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;
}
