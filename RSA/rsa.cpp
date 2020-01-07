#include<iostream>
#include<stdlib.h>
#include<bitset>
#include<time.h>
#include<math.h>
using namespace std;

#define TRUE 1
#define FALSE 0

typedef bitset<32> DWord;

unsigned long N;

#define randA2B(a, b) (rand() % (b-a+1))+ a;

// 自定义取模函数，防止大数溢出 (a^q)mod(m)
unsigned long modN(long a, long q) {
  unsigned long addition = 1;
  unsigned long temp;
  a = fmod(a, N);
  while (q > 1) {
    temp = fmod(pow(a, 2), N);
    if (q%2 == 1)
      addition = fmod((addition*a), N);
    q = q/2;
    a = temp;
  }
  return fmod((temp*addition), N);
}

// 测似乎以a为底时，n是否为合数
int is_composite(unsigned long a, unsigned long d, unsigned long s) {
  if (modN(a, d) == 1)
    return FALSE;
  for (int i=0; i<s; i++) {
    if (modN(a, pow(2, i)*d) == (N-1))
      return FALSE;
  }
  return TRUE;
}

/** 
 *            Miller-Rabin素性测试
 * 要测试N是否为素数，首先将N−1分解为(2^s)d。
 * 在每次测试开始时，先随机选一个介于[1,N−1]的整数a，之后如果对所有的r∈[0,s−1]，
 * 若(a^d)modN≠1且(a^(2^r))*dmodN≠−1，则N是合数。否则，N有3/4的概率为素数。
 */
int is_probable_prime(unsigned long n, int accuracy=5) {
  if (n < 2)
  	return FALSE; 
  if (n == 2) // 2是素数，返回真
    return TRUE;
  if (n%2 == 0) // 偶数不是素数
    return FALSE;
  
  // 把n-1写成(2^s)*d的形式
  int s = 0;
  unsigned long d = n-1;
  while(TRUE) {
    int remainder = d%2;
    if (remainder == 1)
      break;
    s += 1;
    d = d/2;
  }

  srand((unsigned)time(NULL));
  for (int i=0; i<accuracy; i++) {
    unsigned long randNum = randA2B(2, n-1);
    if (is_composite(randNum ,d, s))
      return FALSE;
  }
  return TRUE;
}

// 产生一个长度为n的大素数,n最大为32
unsigned long Pseudoprime(int n=32) {
  DWord temp;
  
  srand((unsigned)time(NULL));
  while (TRUE) {
  	for (int i=(n-1); i>0; i--) {
      temp[i] = randA2B(0, 1);
    }
    temp[0] = 1;
  
    N = temp.to_ulong();
    if (is_probable_prime(N))
      return N;
  }
}

int main() {
	
	unsigned long a = Pseudoprime();
	cout<<a;
	return 0;
}
