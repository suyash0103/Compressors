#include<iostream>
#include<fstream>
#include<unordered_map>
#include<string>
using namespace std;

void constructFlag(unordered_map<char,int> flag){
    int arr[]={0,15,32,47,58,64,92,96,123,127};
    int length=sizeof(a)/sizeof(*a);
    int k=0;
    for(int i=0;i<length/2;i++){
        for(int j=arr[i*2];j<arr[i*2+1];j++){
            flag[j]=k;
            k++;
        }
    }
}

void Encode(char ipfile[], char opfile[], char dictionary[]){
    ifstream fin(ipfile, ios::in);
    ofstream o(opfile, ios::binary);
    ifstream codefile(dictionary, ios::in);

    unordered_map<string,int> encodetable;
    unordered_map<char,int> flag;
    constructFlag(flag);
    string present;
    int i=0;
    while (!codefile.eof()) {
        codefile>>present;
        encodetable[present]=i;
        i++;
    }

    //encoding



    fin.close();
    o.close();
    codefile.close();
}
int main(int argc, char *argv[]) {
    if(argc!=4){
        cout<<"Correct usage: ./a.out ipfile opfile codebook";
        exit(0);
    }
    Encode(argv[1], argv[2], argv[3]);
}