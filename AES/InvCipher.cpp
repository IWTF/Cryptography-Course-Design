#include "globals.h"
#include "util.h"

/* 字节代替，非线性变换
这里通过S_Box实现
前两个字节对应的16进制数作为行号，后两个字节对应的16进制数作为列号
*/
void InvSubBytes(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      InvSubSingleByte(state[i][j]);
    }
  }
};


/*
行移位
第0行不移位保持不变；第1行左移1个字节；第2行左移2个字节；第3行左移3个字节
*/
void InvShiftRows(byte state[4][4]) {
  for (int i=1; i<4; i++) {
    byte temp[4] = state[i];
    for (int j=0; j<4; j++) {
      int t=j-i;
      if ((j-i) < 0)
        t = 4+j-i;
      state[j] = temp[t];
    }
  }
};

/* 列混合变换 */
void InvMixColumns(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    int s0=state[0][i].to_ulong();
    int s1=state[1][i].to_ulong();
    int s2=state[2][i].to_ulong();
    int s3=state[3][i].to_ulong();
    
    // 因为实现了矩阵 乘2，乘3的运算，所以大数运算靠小数加法实现
    // 例： 15·s0 = 8·s0 ^ 4·s0 ^ 2·s0
    // 又因为矩阵乘法特殊，**加法为异或** 上式 可以化简为 15*s0 = 3*s0
    // 可知： 0e = 1000^0100^0010 |  09 = 1000^0001  |  0d = 1000^0100  | 0b = 1000^0010^0001
    
    state[0][i] = (GFMul(8, s0)^GFMul(4, s0)^GFMul(2, s0))^(GFMul(8, s1)^GFMul(2, s1)^s1)^(GFMul(8, s2)^GFMul(4, s2))^(GFMul(8, s3)^s3);
    state[1][i] = (GFMul(8, s1)^GFMul(4, s1)^GFMul(2, s1))^(GFMul(8, s2)^GFMul(2, s2)^s2)^(GFMul(8, s3)^GFMul(4, s3))^(GFMul(8, s0)^s0);
    state[2][i] = (GFMul(8, s2)^GFMul(4, s2)^GFMul(2, s2))^(GFMul(8, s3)^GFMul(2, s3)^s3)^(GFMul(8, s0)^GFMul(4, s0))^(GFMul(8, s1)^s1);
    state[3][i] = (GFMul(8, s3)^GFMul(4, s3)^GFMul(2, s3))^(GFMul(8, s0)^GFMul(2, s0)^s0)^(GFMul(8, s1)^GFMul(4, s1))^(GFMul(8, s2)^s2);
  }
};

void InvCipher(state in[16], word w[Nk*(Nr+1)]) {
  byte state[4][4];
  strToBytes(in.to_string(), state);
  AddRoundKey(state, w, 0);
  for (int i=1; i<Nr; i++) {
    InvShiftRow(state);
    InvSubBytes(state);
    AddRoundKey(state, w, i*Nk);
    InvMixColumns(state);
  }
  InvShiftRow(state);
  InvSubBytes(state);
  AddRoundKey(state, w, i*Nk);

  // 将结果输入到文件
}