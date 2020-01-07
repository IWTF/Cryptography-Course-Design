/*
* 加密函数
*/
#ifndef _INVCIPHER_H_
#define _INVCIPHER_H_


// 字节替换
void SubBytes(byte state[4][4]);

// 行变换
void ShiftRows(byte state[4][4]);

// 列混合变换
void MixColumns(byte state[4][4]);

// 圈密钥加法变换 加密/解密一样，所以放到了util
void Cipher(byte in[16], word w[]);

#endif
