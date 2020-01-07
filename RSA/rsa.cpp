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

// �Զ���ȡģ��������ֹ������� (a^q)mod(m)
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

// ���ƺ���aΪ��ʱ��n�Ƿ�Ϊ����
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
    if (is_composite(randNum ,d, s))
      return FALSE;
  }
  return TRUE;
}

// ����һ������Ϊn�Ĵ�����,n���Ϊ32
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
