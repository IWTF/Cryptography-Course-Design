/*
AES的分组长度为128比特，对于218比特的输入，经过加密/解密变换后，输出也是128比特
AES的各运算都是以字节为单位的，即 128比特 划分为 16各字节
z表示输入比特，t表示字节，s表示中间状态的比特
*/
#include<iostream>
#include<bitset>
#include<string>
using namespace std;
#include "globals.h"
#include "enUtil.cpp"

int main() {
  string b = strval("12345678123456781234567812345678");
  byte a[2][2];
  for (int j=0; j<4; j++) {
    for (int i=0; i<4; i++) {
		a[i][j](b, 0, 8);
    }
  }
}
