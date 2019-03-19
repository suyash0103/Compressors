#include<iostream>
#include<fstream>
#include <map>
using namespace std;

void lzw(char file[]){
    ifstream fin;
    fin.open(file, ios::in);

    //maping characters of lenght 1
    int map_size=4096;
    map<string,int> encodetable;
    map<int,string> decodetable;
    int map_current=256;
    for (int i = 0; i < 256; i++){
        decodetable[i] = std::string(1, i);
        encodetable[std::string(1, i)]=i;
    }

    char p,c;int noofcharinput,noofcharoutput;
    if(!fin.eof()) fin.get(p); 
    string present;
    present=p;
	while (!fin.eof() ) {
        noofcharinput++;
        fin.get(c);
        if(encodetable.find(present+c)!=encodetable.end()){
            present=present+c;
        } else if(map_current<4096){
            noofcharoutput++;
            cout<<present<<" "<<encodetable[present]<<endl;
            encodetable[present+c]=map_current;
            decodetable[map_current]=present+c;
            map_current++;
            present=c;
        }else{
            noofcharoutput++;
            cout<<present<<" "<<encodetable[present]<<endl;
            present=c;
        }
	}
    fin.close();
    cout<<"noofcharinput:"<<noofcharinput<<endl;
    cout<<"noofcharoutput:"<<noofcharoutput<<endl;
}

int main(int argc, char *argv[]) {
   lzw(argv[1]);
}