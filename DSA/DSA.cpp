
#include <cassert>
#include <sstream>
#include <ctime>
#include "DSA.h"
#include <iostream>

/**
 * 函数功能:初始化DSA对象的相关信息
 * 参数含义:len表示大素数的二进制位数
 */
void DSA::init(const unsigned len) {
    srand((unsigned)time(NULL));
    // 产生大素数p
    p = createPrime(len, 15);// 出错概率为(1/4)^15
    
    BigInteger temp = p-1;
	// 生成p-1的素因子q 
    PrimeFactorGen();
	
	BigInteger h = createRandomSmaller(p);
	g = h.modPow(temp.divide(q), p);
    
    // 产生加密/解密密钥 
    createExponent(); 
}

void DSA::PrimeFactorGen() {// 160位2进制数，等于40位16进制数
    static const char hex_table[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	BigInteger temp = p-1;
                               
	std::ostringstream oss, ossMax;
	oss << hex_table[8];  // 生成160位的最小奇数 
	ossMax << hex_table[15];
    for (size_t i=1; i<39; ++i) {
    	oss << hex_table[0];
    	ossMax << hex_table[15];
	}  
    oss << hex_table[1];// 最后一位为奇数
    ossMax << hex_table[15];
    
    q = BigInteger(oss.str());
    BigInteger end(ossMax.str());
    
    for (; q<end; q=q+2) {
    	if (temp.mod(q) != BigInteger("0"))
    		if (isPrime(q, 15))
    			break;
	}
}

/**
 * 函数功能:签名算法
 */
void DSA::signature(const BigInteger & m, BigInteger & r, BigInteger & s) {
    k = createOddNum(100);
	r = g.modPow(k, p).mod(q);
    s = (k.modInverse(q)*(m+x*r).mod(q)).mod(q);
}

/**
 * 函数功能:验证算法
 */
int DSA::verification(const BigInteger & m, BigInteger & r, BigInteger & s, BigInteger & w, BigInteger & u1, BigInteger & u2) {
	w = s.modInverse(q);
	u1 = (m.multiply(w)).mod(q);
	u2 = (r.multiply(w)).mod(q);
	
	BigInteger temp1;
	BigInteger temp2;
	temp1 = g.modPow(u1, p);
	temp2 = y.modPow(u2, p);
	temp2 = temp1.multiply(temp2).mod(p);
	if (temp2.mod(q) == r)
		return 1;
	else
		return 0;
}

/**
 * 函数功能:输出DSA相关数据
 * 参数含义:out表示输出流,rsa表示要输出的DSA对象
 */
std::ostream & operator << (std::ostream & out, const DSA & dsa) {
    out << "p: " << dsa.p << "\n";
    out << "q: " << dsa.q << "\n";
    out << "g: " << dsa.g << "\n";
    out << "x: " << dsa.x << "\n";
    out << "y: " << dsa.y;
    return out;
}

/**
 * 函数功能:生成一个长度为len的奇数
 * 参数含义:len代表奇数的二进制长度
 */
BigInteger DSA::createOddNum(unsigned len) {
    static const char hex_table[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    len >>= 2;    // 十六进制数据,每位占4位二进制
    if (len) {
        std::ostringstream oss;
        for (size_t i=0; i<len-1; ++i)
            oss << hex_table[rand()%16];
        oss << hex_table[1];// 最后一位为奇数
        return BigInteger(oss.str());
    }
    return BigInteger("F");
}

/**
 * 函数功能:判断一个数是否为素数,采用米勒拉宾大素数检测算法,失误率为(1/4)^k
 * 参数含义:num代表要判定的数,k代表测试次数
 */
bool DSA::isPrime(const BigInteger & num, const unsigned k) {
    assert(num != BigInteger::ZERO);// 测试num是否为0
    if (num == BigInteger::ONE)
        return false;    // 1不是素数
    if (num == BigInteger::TWO)
        return true;    // 2是素数

    BigInteger t = num-1;
    BigInteger::bit b(t);// 二进制数
    if (b.at(0) == 1)    // 减一之后为奇数,原数为偶数
        return false;
    // num-1 = 2^s*d
    size_t s = 0;    // 统计二进制末尾有几个0
    BigInteger d(t);
    for (size_t i=0; i<b.size(); ++i) {
        if (!b.at(i)) {
            ++s;
            d = d.shiftRight(1);// 计算出d
        }
        else
            break;
    }

    for (size_t i=0; i<k; ++i) {// 测试k次
        BigInteger a = createRandomSmaller(num);// 生成一个介于[1,num-1]之间的随机数a
        BigInteger x = a.modPow(d, num);
        if (x == BigInteger::ONE)// 可能为素数
            continue;
        bool ok = true;
        // 测试所有0<=j<s,a^(2^j*d) mod num != -1
        for (size_t j=0; j<s && ok; ++j) {
            if (x == t)
                ok = false;    // 有一个相等,可能为素数
            x = x.multiply(x).mod(num);
        }
        if (ok)    // 确实都不等,一定为合数
            return false;
    }
    return true;    // 通过所有测试,可能为素数
}

/**
 * 函数功能:随机生成一个比val小的数
 * 参数含义:val代表比较的那个数
 */
BigInteger DSA::createRandomSmaller(const BigInteger & val) {
    BigInteger::base_t t = 0;
    do {
        t = rand();
    } while (t == 0);// 随机生成非0数

    BigInteger mod(t);
    BigInteger ans = mod%val;    // 比val要小
    if (ans == BigInteger::ZERO)// 必须非零
        ans = val-BigInteger::ONE;
    return ans;
}

/**
 * 函数功能:生成一个二进制长度为len的大素数
 * 参数含义:len代表大素数的长度,k代表素数检测的次数
 */
BigInteger DSA::createPrime(unsigned len, const unsigned k) {
    assert(k > 0);
    BigInteger ans = createOddNum(len);// 先生成一个奇数
    while (!isPrime(ans, k)) {// 素性检测
        ans = ans.add(BigInteger::TWO);// 下一个奇数
    }
    return ans;
}

/**
 * 函数功能:根据提供的欧拉数生成公钥、私钥指数
 * 参数含义:eul表示提供的欧拉数
 */
void DSA::createExponent() {
    x = createRandomSmaller(q);
    y = g.modPow(x, p);
}
