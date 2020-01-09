#ifndef __ENCRYPTDECRYPT_H__
#define __ENCRYPTDECRYPT_H__

#include <string>
#include "DSA.h"
#include "SHA1.h" 
class EncryptDecrypt {
public:
    EncryptDecrypt() {}
    ~EncryptDecrypt() {}

    void menu();    // �˵���ʾ
    bool encrypt();    // ����
    bool decrypt();    // ����
    void print();    // ��ӡRSA�����Ϣ
    void reset();    // ����RSA�����Ϣ
    BigInteger r;    // ǩ�������Ĳ���
    BigInteger s;
    BigInteger w;
    BigInteger u1;
    BigInteger u2;
protected:
    void load(int);    // ���ݸ���λ������RSA����
    bool islegal(const std::string &);// �ж������ַ����Ƿ�Ϸ�
private:
    DSA dsa;
    SHA1_CTX ctx;
    BigInteger m;  // Ҫ����ǩ������Ϣ
};

#endif // __ENCRYPTDECRYPT_H__
