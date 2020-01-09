
#ifndef __DSA_H__
#define __DSA_H__

#include <ostream>
#include "BigInteger.h"
class DSA {
public:
    DSA() {}
    DSA(const unsigned len) { init(len); }    // ����len��ʼ������
    ~DSA() {}

    void init(const unsigned);// ��ʼ��,����������p 

    void PrimeFactorGen();
    void signature(const BigInteger &, BigInteger &, BigInteger &);    // ��Կ����
    int verification(const BigInteger &, BigInteger &, BigInteger &, BigInteger &, BigInteger &, BigInteger &);// ˽Կ����

protected:
    friend std::ostream & operator << (std::ostream &, const DSA &);// ����������
private:
    BigInteger createOddNum(unsigned);// ����һ��������,����Ϊ�䳤��
    bool isPrime(const BigInteger &, const unsigned);// �ж��Ƿ�Ϊ����
    BigInteger createRandomSmaller(const BigInteger &);// �������һ����С����
    BigInteger createPrime(unsigned, const unsigned);// ����һ��������,����Ϊ�䳤��
    void createExponent();// �����ṩ��ŷ�������ɹ�Կ��˽Կָ��
public:
    BigInteger y, g;// ��Կ
private:
    BigInteger x, k;// ˽Կ
    BigInteger p, q;// ������p��q
};

#endif // __RSA_H__	
