#include "../include/globals.h"

#define splitByte(b, x, y)           \
{                                    \
  x=b[7]*8 + b[6]*4 + b[5]*2 + b[4]; \
  y=b[4]*8 + b[2]*4 + b[1]*2 + b[0]; \
}
/* 非线性变换
这里通过S_Box实现
前两个字节对应的16进制数作为行号，后两个字节对应的16进制数作为列号
*/
void SubBytes(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      int x, y;
      splitByte(state[i][j], x, y);
      state[i][j] = S_Box[i][j];
    }
  }
};


void ShiftRows();

void MixColumns();

void AddRoundKey();