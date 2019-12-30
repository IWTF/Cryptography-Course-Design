#include<iostream>
#include<bitset>
#include<string.h>
using namespace std;

typedef bitset<8> byte; 
typedef bitset<32> word;
const int Nb = 4;

extern int m;
//extern FILE* listing; /* 输出文件 */

int GFMul(int a, int b) {
  int temp;
  byte bTemp;
  switch(a) {
    case 2:
      temp = b;
      b = b<<1;
      if (temp/128 > 0)   // 判断b7是否为1
        b = b^0x1b;
      return b;
    case 3:
      temp = b;
      b = b<<1;
      if (temp/128 > 0)
        b = b^0x1b;
      b ^= temp;
      return b;
    case 4:
      bTemp = GFMul(2, b);
      return GFMul(2, bTemp.to_ulong());
    case 8:
      bTemp = GFMul(4, b);
      return GFMul(2, bTemp.to_ulong());
    case 16:
      bTemp = GFMul(8, b);
      return GFMul(2, bTemp.to_ulong());
    default:
      return b;
  }
}

int main() {
	byte a[4][4] = {
		{12, 1, 1, 3},
		{12, 1, 1, 3},
		{12, 1, 1, 3},
		{12, 1, 1, 3},
	};
	
	byte b = GFMul(0x10, 0x57)^GFMul(2, 0x57)^0x57;
//	byte b = GFMul(0x10, 0x57);
	cout<<b;
	return 0;
} 

