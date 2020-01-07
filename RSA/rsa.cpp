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

// �Զ���ȡģ��������ֹ������� (a^q)mod(m)
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

// ���ƺ���aΪ��ʱ��n�Ƿ�Ϊ����
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
 *            Miller-Rabin���Բ���
 * Ҫ����N�Ƿ�Ϊ���������Ƚ�N�6�11�ֽ�Ϊ(2^s)d��
 * ��ÿ�β��Կ�ʼʱ�������ѡһ������[1,N�6�11]������a��֮����������е�r��[0,s�6�11]��
 * ��(a^d)modN��1��(a^(2^r))*dmodN�ف6�11����N�Ǻ���������N��3/4�ĸ���Ϊ������
 */
int is_probable_prime(unsigned long n, int accuracy=5) {
  if (n < 2)
  	return FALSE; 
  if (n == 2) // 2��������������
    return TRUE;
  if (n%2 == 0) // ż����������
    return FALSE;
  
  // ��n-1д��(2^s)*d����ʽ
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

// ����һ������Ϊn�Ĵ�����,n���Ϊ32
unsigned long Pseudoprime(int n=32) {
  DWord temp;
  
  srand((unsigned)time(NULL));
  while (TRUE) {
    // ����һ��������
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

// ��ʼ������һЩС����������ߴ���������Ч�� 
void initMiniPrime() {
	for (int i=0; i<530; i++) {
		if (is_probable_prime(i, 10)) {
			miniPrime[num++] = i;
		}	
	}
}

// ��չŷ������㷨
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
