#include<iostream>
#include<bitset>
#include<string.h>
using namespace std;

typedef bitset<8> byte; 
typedef bitset<32> word;
const int Nb = 4;

extern int m;
//extern FILE* listing; /* ����ļ� */

int main() {
	byte a[4][4] = {
		{12, 1, 1, 3},
		{12, 1, 1, 3},
		{12, 1, 1, 3},
		{12, 1, 1, 3},
	};
	
	cout<<m;
	int b = -2%3;
	cout<<b;
} 

