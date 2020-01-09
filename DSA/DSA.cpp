
#include <cassert>
#include <sstream>
#include <ctime>
#include "DSA.h"
#include <iostream>

/**
 * ��������:��ʼ��DSA����������Ϣ
 * ��������:len��ʾ�������Ķ�����λ��
 */
void DSA::init(const unsigned len) {
    srand((unsigned)time(NULL));
    // ����������p
    p = createPrime(len, 15);// �������Ϊ(1/4)^15
    
    BigInteger temp = p-1;
	// ����p-1��������q 
    PrimeFactorGen();
	
	BigInteger h = createRandomSmaller(p);
	g = h.modPow(temp.divide(q), p);
    
    // ��������/������Կ 
    createExponent(); 
}

void DSA::PrimeFactorGen() {// 160λ2������������40λ16������
    static const char hex_table[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	BigInteger temp = p-1;
                               
	std::ostringstream oss, ossMax;
	oss << hex_table[8];  // ����160λ����С���� 
	ossMax << hex_table[15];
    for (size_t i=1; i<39; ++i) {
    	oss << hex_table[0];
    	ossMax << hex_table[15];
	}  
    oss << hex_table[1];// ���һλΪ����
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
 * ��������:ǩ���㷨
 */
void DSA::signature(const BigInteger & m, BigInteger & r, BigInteger & s) {
    k = createOddNum(100);
	r = g.modPow(k, p).mod(q);
    s = (k.modInverse(q)*(m+x*r).mod(q)).mod(q);
}

/**
 * ��������:��֤�㷨
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
 * ��������:���DSA�������
 * ��������:out��ʾ�����,rsa��ʾҪ�����DSA����
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
 * ��������:����һ������Ϊlen������
 * ��������:len���������Ķ����Ƴ���
 */
BigInteger DSA::createOddNum(unsigned len) {
    static const char hex_table[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    len >>= 2;    // ʮ����������,ÿλռ4λ������
    if (len) {
        std::ostringstream oss;
        for (size_t i=0; i<len-1; ++i)
            oss << hex_table[rand()%16];
        oss << hex_table[1];// ���һλΪ����
        return BigInteger(oss.str());
    }
    return BigInteger("F");
}

/**
 * ��������:�ж�һ�����Ƿ�Ϊ����,����������������������㷨,ʧ����Ϊ(1/4)^k
 * ��������:num����Ҫ�ж�����,k������Դ���
 */
bool DSA::isPrime(const BigInteger & num, const unsigned k) {
    assert(num != BigInteger::ZERO);// ����num�Ƿ�Ϊ0
    if (num == BigInteger::ONE)
        return false;    // 1��������
    if (num == BigInteger::TWO)
        return true;    // 2������

    BigInteger t = num-1;
    BigInteger::bit b(t);// ��������
    if (b.at(0) == 1)    // ��һ֮��Ϊ����,ԭ��Ϊż��
        return false;
    // num-1 = 2^s*d
    size_t s = 0;    // ͳ�ƶ�����ĩβ�м���0
    BigInteger d(t);
    for (size_t i=0; i<b.size(); ++i) {
        if (!b.at(i)) {
            ++s;
            d = d.shiftRight(1);// �����d
        }
        else
            break;
    }

    for (size_t i=0; i<k; ++i) {// ����k��
        BigInteger a = createRandomSmaller(num);// ����һ������[1,num-1]֮��������a
        BigInteger x = a.modPow(d, num);
        if (x == BigInteger::ONE)// ����Ϊ����
            continue;
        bool ok = true;
        // ��������0<=j<s,a^(2^j*d) mod num != -1
        for (size_t j=0; j<s && ok; ++j) {
            if (x == t)
                ok = false;    // ��һ�����,����Ϊ����
            x = x.multiply(x).mod(num);
        }
        if (ok)    // ȷʵ������,һ��Ϊ����
            return false;
    }
    return true;    // ͨ�����в���,����Ϊ����
}

/**
 * ��������:�������һ����valС����
 * ��������:val����Ƚϵ��Ǹ���
 */
BigInteger DSA::createRandomSmaller(const BigInteger & val) {
    BigInteger::base_t t = 0;
    do {
        t = rand();
    } while (t == 0);// ������ɷ�0��

    BigInteger mod(t);
    BigInteger ans = mod%val;    // ��valҪС
    if (ans == BigInteger::ZERO)// �������
        ans = val-BigInteger::ONE;
    return ans;
}

/**
 * ��������:����һ�������Ƴ���Ϊlen�Ĵ�����
 * ��������:len����������ĳ���,k�����������Ĵ���
 */
BigInteger DSA::createPrime(unsigned len, const unsigned k) {
    assert(k > 0);
    BigInteger ans = createOddNum(len);// ������һ������
    while (!isPrime(ans, k)) {// ���Լ��
        ans = ans.add(BigInteger::TWO);// ��һ������
    }
    return ans;
}

/**
 * ��������:�����ṩ��ŷ�������ɹ�Կ��˽Կָ��
 * ��������:eul��ʾ�ṩ��ŷ����
 */
void DSA::createExponent() {
    x = createRandomSmaller(q);
    y = g.modPow(x, p);
}
