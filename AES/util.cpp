#include "../include/globals.h"

// 把biset类型变量转int
#define bit2Int(byte b) b.to_ulong()

// 将字符串转为byte类型的4*4数组
byte str2Bytes(string str, byte w[4][4]) {
  int beg = 0;
  for(int j=0; j<4; j++) {
    for (int i=0; i<4; i++) {
      byte temp(str.substr(beg*8, 8));
      w[i][j] = temp;
      beg++;
    }
  }
}

// SubBytes的辅助函数，将一个字节的前4bit赋给x，后4bit赋给y
void splitByte(byte b, int x, int y) {
  x=b[7]*8 + b[6]*4 + b[5]*2 + b[4];
  y=b[3]*8 + b[2]*4 + b[1]*2 + b[0];
}

// 单个字节变换，对应S盒
string SubSingleByte(byte &b) {
  int x, y;
  splitByte(b, x, y);
  b = S_Box[x][y];
  return b.to_string();
}

// 单个字节变换，对应逆S盒
string InvSubSingleByte(byte &b) {
  int x, y;
  splitByte(b, x, y);
  b = Inv_S_Box[x][y];
  return b.to_string();
}


// 对字中的每一个字节进行subBytes
word SubWord(word w) {
  // 先将word转为string，并化分为一个个字节
  string str = w.to_string();
  byte b1(str, 0, 8);
  byte b2(str, 8, 8);
  byte b3(str, 16, 8);
  byte b4(str, 24, 8);
  
  string targetStr = SubSingleByte(b1) + SubSingleByte(b2) + SubSingleByte(b3) + SubSingleByte(b4);
  word ret(targetStr);
  return ret;
}

// 对输入字的循环置换，左移一个字节
word RotWord(word w) {
  // 保存其高8位, 并将其转为int
  string str = w.to_string();
  byte temp(str, 0, 8);
  int last = bit2Int(temp);

  // 舍去高8位，并将其转为整型
  int pre = bit2Int(w<<8);

  // 将原word循环左移一个字节，并将高8位或到最后
  word ret = (pre | temp);
  return ret;
}

/* 密钥扩展算法 */
void KeyExpansion(byte key[4*Nk], word w[4*(Nr+1)], int Nk) {
  int i=0;
  for (; i<Nk; i++)
    w[i] = bit2Int(k[4*i])<<3 + bit2Int(k[4*i+1])<<2 + bit2Int(k[4*i+2])<<1 + bit2Int(k[4*i+3]);
  while(i < 4*(Nr+1)) {
    temp = w[i-1];
    if (i%Nk == 0)
      temp = SubWord(RotWord(temp)) ^ Rcon[i/Nk];
    else if ((Nk > 6) && (i%Nk == 4))
      temp = SubWord(temp);
    w[i] = w[i-Nk]^temp;
    i += 1;
  }
}

/* 全密钥加法变换 
* 传入扩展密钥的地址
* 传入此轮循环所需密钥的 起始地址
* 一次循环中，使用到的密钥长度为128
*/
void AddRoundKey(byte state[4][4], word w[], int beg) {
  string str = w.to_srting().substr(beg, 128);
  byte temp[4][4];
  str2Bytes(str, temp);
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      state[i][j] = state[i][j]^temp[i][j];
    }
  }
};

/*
* 矩阵元素的乘法和加法都是定义在基于GF(2^8)上的二元运算,并不是通常意义上的乘法和加法。
* 这里涉及到一些信息安全上的数学知识，不过不懂这些知识也行。其实这种二元运算的加法等价于两个字节的异或
* 乘法则复杂一点。对于一个8位的二进制数来说，
** 乘2时，使用域上的乘法乘以(00000010)等价于左移1位(低位补0)后，再根据情况同(00011011)进行异或运算（当最高位为1时）
** 乘3时，乘以(0000 0011)可以拆分成先分别乘以(0000 0001)和(0000 0010)，再将两个乘积异或(已经过验证)
*/
int GFMul(int a, int b) {
  if (a == 2) {
    b = b<<1;
    if (a/128 > 0)
      b = b^0x00011011;
  } else {
    int temp = b;
    b = b<<1;
    if (a/128 > 0)
      b = b^0x00011011;
    b ^= temp;
  }
  return b;
}