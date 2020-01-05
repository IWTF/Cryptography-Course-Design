#include<iostream>
#include<fstream>
#include<bitset>
#include<string.h>
using namespace std;

typedef bitset<8> byte; 
typedef bitset<32> word;
const int Nb = 4;

extern int m;
//extern FILE* listing; /* Êä³öÎÄ¼þ */

int main() {
	byte a[4][4] = {
		{12, 1, 1, 3},
		{12, 1, 1, 3},
		{12, 1, 1, 3},
		{12, 1, 1, 3},
	};
	
	ifstream infile;
	infile.open("message.txt");
	byte in[16];
	int i=0;
	infile>>in[i++];
	while(infile) {
      infile>>in[i++];
      if (i%16 == 0) {
        for (int j=0; j<16; j++)
      		cout<<in[j];
      	cout<<"\n";
        i=0;
      }
    } // end while

	return 0;
} 

