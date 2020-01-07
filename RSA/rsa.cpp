#include<iostream>
#include<stdlib.h>
#include<bitset>
#include<time.h>
#include<math.h>
using namespace std;

#define TRUE 1
#define FALSE 0

typedef bitset<32> DWord;

unsigned long miniPrime[100];
int num = 0;

#define randA2B(a, b) (rand() % (b-a+1))+ a;

// 自定义取模函数，防止大数溢出 (a^q)mod(m)
unsigned long modN(long a, long q, int n) {
  unsigned long addition = 1;
  unsigned long temp;
  a = fmod(a, n);
  while (q > 1) {
    temp = fmod(pow(a, 2), n);
    if (q%2 == 1)
      addition = fmod((addition*a), n);
    q = q/2;
    a = temp;
  }
  return fmod((temp*addition), n);
}

// 测似乎以a为底时，n是否为合数
int is_composite(unsigned long a, unsigned long d, unsigned long s, unsigned long n) {
  if (modN(a, d, n) == 1)
    return FALSE;
  for (int i=0; i<s; i++) {
    if (modN(a, pow(2, i)*d, n) == (n-1))
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
    if (is_composite(randNum ,d, s, n))
      return FALSE;
  }
  return TRUE;
}

// 产生一个长度为n的大素数,n最大为32
unsigned long Pseudoprime(int n=32) {
  DWord temp;
  
  srand((unsigned)time(NULL));
  while (TRUE) {
    // 产生一个大奇数
  	for (int i=(n-1); i>0; i--) {
      temp[i] = randA2B(0, 1);
    }
    temp[0] = 1;
  
    unsigned long N = temp.to_ulong();
    int flag = 0;
    for (int i=0; i<99; i++) {
    	if (fmod(N, miniPrime[i]) == 0) {
    		flag = 1;
			  break;
		  }
	  }
	  if (flag == 1)
	  	continue;
    if (is_probable_prime(N))
      return N;
  }
} 

// 初始化生成一些小的素数，提高大素数产生效率 
void initMiniPrime() {
	for (int i=0; i<530; i++) {
		if (is_probable_prime(i, 10)) {
			miniPrime[num++] = i;
		}	
	}
}

// 扩展欧几里得算法
int Extended_Euclid(int a,int &x,int b,int &y,int c) {
  if(b==0){x=c/a,y=0;return a;}
  else
  {
    int p=Extended_Euclid(b,x,a%b,y,c);
    int x_=x,y_=y;
    x=y_; y=x_-a/b*y_; 
    return p;
  }
}

int main() {
	initMiniPrime();
	
	int a = Pseudoprime(18);
	cout<<a;
	return 0;
}
