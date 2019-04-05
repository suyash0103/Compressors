#include<iostream>
#include<fstream>
#include<unordered_map>
#include<string>
#include<ctime>
#include <chrono>

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

void prepare_buffer(uint64_t &buff, ofstream &o, int code_32, int bit_length, short int capital = 0) {
    uint64_t code;
    code = static_cast<uint64_t>(code_32);
    if (bit_length == 19) {
        //for 19 bit code 1st bit is 1
        if (capital) {
            code = code | (3 << 17);
        } else code = code | (1 << 18);
        //writing to file
        if (buff_write(&buff, code, 19, 0) == 0) {
            o.write((char *) &buff, sizeof(buff));
            buff = 0;
            buff_write(&buff, code, 19, 0);
        }
    }
    if (bit_length == 9) {
        //for 9 bit code 9th bit is 0
        if (buff_write(&buff, code, 9, 0) == 0) {
            o.write((char *) &buff, sizeof(buff));
            buff = 0;
            buff_write(&buff, code, 9, 0);
        }
    }
}

const int ALPHABET_SIZE = 26;

// trie node
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    uint16_t id;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void) {
    struct TrieNode *pNode = new TrieNode;

    pNode->id = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;

    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void insert(struct TrieNode *root, string key,uint16_t id) {
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++) {
        int index = key[i] - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }
    pCrawl->id = id;
}

// Returns the id of node
uint16_t search(struct TrieNode *root, string key) {
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++) {
        int index = key[i] - 'a';
        if(index<0) return 0;
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
    }

    if(pCrawl != NULL) return pCrawl->id;
    else return 0;
}

void Encode(char ipfile[], char opfile[], char dictionary[]) {
    ifstream fin(ipfile, ios::in);
    ofstream o(opfile, ios::binary);
    ifstream codefile(dictionary, ios::in);
    string present;
    int i = 1; //id starts from 1 , 0 if word not present

    struct TrieNode *trie= getNode();
    while (!codefile.eof()) {
        codefile >> present;
        insert(trie, present,i);
        i++;
    }

    //encoding
    string p;
    char c;
    short int capital = 0;
    uint16_t id=0;
    int count = 0;
    uint64_t buff = 0;

    while (fin.get(c)) {
        if (c == 0) {
            id=search(trie,p);
            //cout<<p<<" "<<id<<endl;
            if (id != 0) {
                count++;
                prepare_buffer(buff, o, id, 19, capital);
            } else if (p.length() >= 1) {
                if (capital)p[0] = (char) toupper(p[0]);
                for (int i = 0; i < p.length(); i++) {
                    prepare_buffer(buff, o, int(p[i]), 9);
                }
            }
            capital = 0;
            p.clear();
        } else {
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
    codefile.close();
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Correct usage: ./a.out tokens opfile dictionary";
        exit(0);
    }
    auto start = high_resolution_clock::now();
    Encode(argv[1], argv[2], argv[3]);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;
}