#include<iostream>
#include<fstream>
#include<unordered_map>
#include<string>
#include<ctime>
#include<chrono>

using namespace std::chrono;
using namespace std;

/**
 * @brief Write data into buffer.
 * buff_read @author srinskit
 * Note that when function returns 0, the function has to be recalled with new
 * buffer.
 *
 * @param buff The pointer to the buffer to be written to.
 * @param code The data to be written.
 * @param code_len The length of data to be written in bits.
 * @param flush Set to 1 to flush buffer and reset function.
 * @return int -1 if error, 1 if success, 0 if function needs to be recalled
 * with next buffer.
 * @return int 1 if flush didn't modify buffer, 0 if flush modified buffer.
 */
int buff_write(uint64_t *buff, uint64_t code, unsigned code_len, int flush) {
    const unsigned buff_len = 8 * sizeof(*buff);
    static unsigned vacant_len = buff_len;
    static unsigned to_write_len = 0;
    if (code_len > buff_len) return -1;
    if (flush) {
        if (vacant_len == buff_len) return 0;
        *buff <<= vacant_len;
        int ret = buff_len - vacant_len;
        vacant_len = buff_len;
        to_write_len = 0;
        return ret;
    }
    if (to_write_len != 0) {
        *buff = code - ((code >> to_write_len) << to_write_len);
        vacant_len -= to_write_len;
        to_write_len = 0;
        return 1;
    }
    if (code_len <= vacant_len) {
        *buff <<= code_len;
        *buff += code;
        vacant_len -= code_len;
        return 1;
    }
    to_write_len = code_len - vacant_len;
    *buff <<= vacant_len;
    *buff += code >> to_write_len;
    vacant_len = buff_len;
    return 0;
}


void prepare_buffer(uint64_t &buff, ofstream &o, ofstream &o2, int code_32, int bit_length, short int capital = 0) {
    uint64_t code=code_32;
    char delimiter=-125;
    if (bit_length == 18) {
        //for 18 bit code 1st bit is 1
        if (capital) {
            code = code | (1 << 17);
        }
        //writing to file
        o2.write(&delimiter, sizeof(delimiter));
        if (buff_write(&buff, code, 18, 0) == 0) {
            o.write((char *) &buff, sizeof(buff));
            buff = 0;
            buff_write(&buff, code, 18, 0);
        }
    }
    if (bit_length == 8) {
        //for 9 bit code 9th bit is 0
        o2.write((char*)&code,sizeof(char));
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
                prepare_buffer(buff, o, o2, table[p], 18, capital);
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

    if (buff_write(&buff, 0, 0, 1) != 0)
        o.write((char *) &buff, sizeof(buff));

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
