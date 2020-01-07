/* 
* 将 加密/解密 变换中共同用到的工具函数抽离，增加复用性
*/
typedef bitset<8> byte;
typedef bitset<32> word;

#ifndef _UTIL_H_
#define _UTIL_H_

#include<string.h>
using namespace std;



extern ifstream infile;
extern ofstream outfile;
extern int Nb;
extern int Nk;
extern int Nr;

// 密钥扩展
void KeyExpension(word key, word w[Nk*(Nr+1)]);

// 辅助函数
byte str2Bytes(string str, byte w[4][4]);
void splitByte(byte b, int x, int y);

// 字节变换辅助函数
string SubSingleByte(byte &b);      // 在SubWord函数里面也有引用
string InvSubSingleByte(byte &b);   // 查找逆S盒

// 圈密钥加法变换
word SubWord(word w);
word RotWord(word w);
void KeyExpansion(byte key[4*Nk], word w[4*(Nr+1)], int Nk);
void AddRoundKey(byte state[4][4], word w[], int beg);

// 列混合变换
int GFMul(int a, int b);

#endif
