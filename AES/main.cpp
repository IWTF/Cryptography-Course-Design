#include<iostream>
using namespace std;

#include "Cipher.h"
#include "InvCipher.h"

/* 输入文件和输出文件 */
ifstream infile;
ofstream outfile;

int main() {
  KeyExpension();

  byte in[16];

  int flag = 1;
  cout<<"1. 加密操作 | 2. 解密操作(1)：";
  cin>>flag;

  char filename[100];
  cout<<"请输入要加/解密的文件名：";
  cin>>fileName;

  infile.open(filename);

  int i=0;
  if (flag == 1) {    // 加密操作
    char data;
    outfile.open("Cipher.txt");

    infile>>data;
    while(infile) {
      in[i++] = data;
      infile>>data;
      if (i%16 == 0) {
        Cipher(in, w);
        i=0;
      }
    } // end while
  } else if(flag == 2) {    // 解密操作
    outfile.open("InvCipher.txt");

    infile>>in[i++];
    while(infile) {
      infile>>in[i++];
      if (i%16 == 0) {
        InvCipher(in, w);
        i=0;
      }
    } // end while
  } // end if

  
  cout<<"\n====== 完成 ======\n";
  return 0;
}