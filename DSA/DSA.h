
#ifndef __DSA_H__
#define __DSA_H__

#include <ostream>
#include "BigInteger.h"
class DSA {
public:
    DSA() {}
    DSA(const unsigned len) { init(len); }    // 利用len初始化对象
    ~DSA() {}

    void init(const unsigned);// 初始化,产生大素数p 

    void PrimeFactorGen();
    void signature(const BigInteger &, BigInteger &, BigInteger &);    // 公钥加密
    int verification(const BigInteger &, BigInteger &, BigInteger &, BigInteger &, BigInteger &, BigInteger &);// 私钥解密

protected:
    friend std::ostream & operator << (std::ostream &, const DSA &);// 输出相关数据
private:
    BigInteger createOddNum(unsigned);// 生成一个大奇数,参数为其长度
    bool isPrime(const BigInteger &, const unsigned);// 判断是否为素数
    BigInteger createRandomSmaller(const BigInteger &);// 随机创建一个更小的数
    BigInteger createPrime(unsigned, const unsigned);// 生成一个大素数,参数为其长度
    void createExponent();// 根据提供的欧拉数生成公钥、私钥指数
public:
    BigInteger y, g;// 公钥
private:
    BigInteger x, k;// 私钥
    BigInteger p, q;// 大素数p和q
};

#endif // __RSA_H__	
