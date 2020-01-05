/*
* 解密函数
*/

#ifndef _INVCIPHER_H_
#define _INVCIPHER_H_

// 逆字节替换
void InvSubBytes(byte state[4][4]); //；

// 逆行变换
void InvShiftRows(byte state[4][4]);

// 逆列混合变换
void InvMixColumns(byte state[4][4]);

// 圈密钥加法变换 加密/解密一样，所以放到了util

// 解密算法
void InvCipher(byte in[16], word w[]);
#endif
