/*
* 存放一些全局变量
* extern其他文件的变量，以供全局使用
*/
#include<fstream>
#include<bitset>
using namespace std;

#ifndef _GLOBAL_H_
#define _GLOBAL_H_


typedef bitset<8> byte;
typedef bitset<32> word;
extern ifstream infile;
extern ofstream outfile;
extern int Nb;
extern int Nk;
extern int Nr;
// S盒
extern byte S_Box[16][16];
// 逆S盒
extern byte Inv_S_Box[16][16];
// 圈密钥加 用到的 常数字数组
extern word Rcon[10];
				 
#endif
