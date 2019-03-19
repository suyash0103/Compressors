#include<iostream>
#include<fstream>
#include <map>
#include<vector>

using namespace std;

struct binary_buffer {
    int buffer = 0;
};

void write_buffer(binary_buffer &buff, ofstream &o, int a, int req = 12) {
    if (buff.buffer == 0) {
        buff.buffer = a;
    } else {
        buff.buffer = buff.buffer << req;
        a = a | buff.buffer;
        o.write((char *) &a, 3);
        buff.buffer = 0;
    }
}

void lzwEncode(char ipfile[], char opfile[], char codefile[]) {
    ifstream fin(ipfile, ios::in);
    ofstream o(opfile, ios::binary);

    //mapping characters of length 1
    int map_size = 4096;
    map<string, int> encodetable;
    vector <string> decodetable;
    int map_current = 255;
    for (int i = 0; i < 256; i++) {
        decodetable.push_back(std::string(1, i));
        encodetable[std::string(1, i)] = i;
    }

    //encoding
    char p, c;
    int noofcharinput = 0, noofcharoutput = 0;
    binary_buffer buf;
    if (!fin.eof()) fin.get(p);
    string present;
    present = p;
    while (!fin.eof()) {
        noofcharinput++;
        fin.get(c);
        if (encodetable.find(present + c) != encodetable.end()) {
            present = present + c;
        } else if (map_current < 4096) {
            noofcharoutput++;
            map_current++;
            //cout<<present<<" "<<encodetable[present]<<endl; //output
            write_buffer(buf, o, encodetable[present]);
            encodetable[present + c] = map_current;
            decodetable.push_back(present + c);
            present = c;
        } else {
            noofcharoutput++;
            //cout<<present<<" "<<encodetable[present]<<endl; //output
            write_buffer(buf, o, encodetable[present]);
            present = c;
        }
    }
    if (buf.buffer != 0)write_buffer(buf, o, 0);

    //writing codebook to file
    int size = 0;
    char null = 0;
    ofstream codebook(codefile, ios::out);
    for (int i = 256; i < decodetable.size(); i++) {
        codebook << decodetable[i] << null;
        size = size + decodetable[i].length();
    }
    cout << "codeBookSize:" << size << endl;
    cout << "noofcharinput:" << noofcharinput << endl;
    cout << "noofcharoutput:" << noofcharoutput << endl;
    codebook.close();
    fin.close();
    o.close();
}

int main(int argc, char *argv[]) {
    lzwEncode(argv[1], argv[2], argv[3]);
}
