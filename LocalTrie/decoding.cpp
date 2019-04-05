#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <chrono>


using namespace std::chrono;
using namespace std;

/**
 * @brief Read data from buffer.
 * buff_read @author srinskit
 * Note that "code" should be read only when function returns 1. When 0 is
 * returned, call the function again with next buffer to complete reading of
 * "code".
 *
 * @param buff The pointer to the buffer to be read from.
 * @param code The pointer to the variable receiving the data.
 * @param code_len The length of data to be read in bits.
 * @param flush Set to 1 to reset function.
 * @return int -1 if error, 1 if success, 0 if function needs to be recalled
 * with next buffer.
 */
int buff_read(uint64_t *buff, uint64_t *code, unsigned code_len, int flush) {
    const unsigned buff_len = 8 * sizeof(*buff);
    static unsigned filled_len = buff_len;
    static unsigned to_read_len = 0;
    if (code_len > buff_len) return -1;
    if (flush) {
        filled_len = buff_len;
        to_read_len = 0;
        return 1;
    }
    if (to_read_len != 0) {
        *code <<= to_read_len;
        *code += *buff >> (buff_len - to_read_len);
        *buff <<= to_read_len;
        filled_len -= to_read_len;
        to_read_len = 0;
        return 1;
    }
    if (code_len <= filled_len) {
        *code = *buff >> (buff_len - code_len);
        *buff <<= code_len;
        filled_len -= code_len;
        return 1;
    }
    to_read_len = code_len - filled_len;
    *code = *buff >> (buff_len - filled_len);
    filled_len = buff_len;
    return 0;
}

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

    string s;
    uint64_t buff = 0,code;
    if (!fin.read((char *) &buff, sizeof(buff))) return;
    while(1){
        code=0;  //read first bit and find if its 19 bit or 9 bit encoding
        if (buff_read(&buff, &code,1, 0) == 0) {
            if (!fin.read((char *) &buff, sizeof(buff))) break;
            buff_read(&buff, &code,1, 0);
        }
        if(code==1){
            code=0;
            if (buff_read(&buff, &code,18, 0) == 0) {
                if (!fin.read((char *) &buff, sizeof(buff))) break;
                buff_read(&buff, &code,18, 0);
            }
            if(code==0)continue;
            if(code & (1<<17)){
                code=code ^ (1<<17);
                s=table[code-1];
                s[0]=toupper(s[0]);
                o<<s;
            }else{
                o<<table[code-1];
            }
        }else{
            if (buff_read(&buff, &code,8, 0) == 0) {
                if (!fin.read((char *) &buff, sizeof(buff))) break;
                buff_read(&buff, &code,8, 0);
            }
            if(code==0)continue;
            o<<(char)code;
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