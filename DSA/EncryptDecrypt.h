#ifndef __ENCRYPTDECRYPT_H__
#define __ENCRYPTDECRYPT_H__

#include <string>
#include "DSA.h"
class EncryptDecrypt {
public:
    EncryptDecrypt() {}
    ~EncryptDecrypt() {}

    void menu();    // 菜单显示
    bool encrypt();    // 加密
    bool decrypt();    // 解密
    void print();    // 打印RSA相关信息
    void reset();    // 重置RSA相关信息
    BigInteger r;    // 签名产生的参数
    BigInteger s;
    BigInteger w;
    BigInteger u1;
    BigInteger u2;
protected:
    void load(int);    // 根据给定位数加载RSA对象
    bool islegal(const std::string &);// 判断输入字符串是否合法
private:
    DSA dsa;
    BigInteger m;  // 要进行签名的消息
};

#endif // __ENCRYPTDECRYPT_H__
