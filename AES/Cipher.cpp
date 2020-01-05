#include "Cipher.h"
#include "globals.h"
#include "util.h"

/* 字节代替，非线性变换
这里通过S_Box实现
前两个字节对应的16进制数作为行号，后两个字节对应的16进制数作为列号
*/
void SubBytes(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      SubSingleByte(state[i][j]);
    }
  }
};


/*
行移位
第0行不移位保持不变；第1行左移1个字节；第2行左移2个字节；第3行左移3个字节
*/
void ShiftRows(byte state[4][4]) {
  for (int i=1; i<4; i++) {
    byte temp[4] = state[i];
    for (int j=0; j<4; j++) {
      state[i][j] = temp[(j+i)%4];
    }
  }
};


/* 列混合变换 */
void MixColumns(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    int s0=state[0][i].to_ulong();
    int s1=state[1][i].to_ulong();
    int s2=state[2][i].to_ulong();
    int s3=state[3][i].to_ulong();
    
    state[0][i] = GFMul(2, s0)^GFMul(3, s1)^s2^s3;
    state[1][i] = GFMul(2, s1)^GFMul(3, s2)^s2^s3;
    state[2][i] = GFMul(2, s2)^GFMul(3, s3)^s2^s3;
    state[3][i] = GFMul(2, s3)^GFMul(3, s0)^s2^s3;
  }
};

void Cipher(byte in[16], word w[Nk*(Nr+1)]) {
  byte state[4][4];
  string str;
  for (int i=0; i<16; i++)
  	str = str + in[i].to_string();
  str2Bytes(str, state);
  // 第一次轮密钥加法运算
  AddRoundKey(state, w, 0);
  // 进行Nr-1次迭代
  for(int i=1; i<Nr; i++) {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(state, w, i*Nk);
  }
  // 最后一次不带MixColumns()
  SubBytes(state);
  ShiftRows(state);
  AddRoundKey(state, w, Nr*Nk);

  // 直接写入文件，就不进行转化了
  // out = state;
}
