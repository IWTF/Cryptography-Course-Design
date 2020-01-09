#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<bitset>
#include<time.h>
#include<math.h>
using namespace std;

#define TRUE 1
#define FALSE 0

typedef bitset<32> DWord;

unsigned long miniPrime[100];
int num = 0;

unsigned long d = 147;  // ������Կ
unsigned long e = 3;  // ������Կ
unsigned long n = 253;  // n=pq

ifstream infile;
ofstream outfile;

#define randA2B(a, b) (rand() % (b-a+1))+ a;

/*****************************���Բ�����غ���*****************************/
/**
 * �Զ���ȡģ��������ֹ������� (a^q)mod(m)
 */
unsigned long modN(long a, long q, int n) {
  unsigned long addition = 1;
  unsigned long temp;
  a = fmod(a, n);
  while (q > 1) {
    temp = fmod(pow(a, 2), n);
    if (q&1)
      addition = fmod((addition*a), n);
    q = q>>1;
    a = temp;
  }
  return fmod((temp*addition), n);
}

/** 
 * ������aΪ��ʱ��n�Ƿ�Ϊ����
 */
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
 * Miller-Rabin���Բ���
 * Ҫ����N�Ƿ�Ϊ���������Ƚ�N-1�ֽ�Ϊ(2^s)d��
 * ��ÿ�β��Կ�ʼʱ�������ѡһ������[1,N-1]������a��֮����������е�r��[0,s-1]��
 * ��(a^d)modN��1��(a^(2^r))*dmodN��1����N�Ǻ���������N��3/4�ĸ���Ϊ������
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

/*****************************������������غ���*****************************/
/**
 * ��ʼ������һЩС����������ߴ���������Ч��
 */ 
void initMiniPrime() {
	for (int i=0; i<530; i++) {
		if (is_probable_prime(i, 10)) {
			miniPrime[num++] = i;
		}	
	}
}

/**
 * ����һ������Ϊn�Ĵ�����,n���Ϊ32
 */
unsigned long Pseudoprime(int n=15) {
  DWord temp;
  
  while (TRUE) {
    // ����һ��������
  	for (int i=(n-1); i>0; i--) {
      temp[i] = randA2B(0, 1);
    }
    temp[0] = 1;
  
    unsigned long N = temp.to_ulong();
//    int flag = 0;
//    for (int i=0; i<99; i++) {
//    	if (fmod(N, miniPrime[i]) == 0) {
//    		flag = 1;
//			  break;
//		  }
//	  }
//	  if (flag == 1)
//	  	continue;
    if (is_probable_prime(N))
      return N;
  }
} 

/********************************���溯��*******************************/
/**
 * ��չŷ������㷨
 * xa+yb=c, ��x��y��ֵ
 */
int Extended_Euclid(unsigned long a,int &x,unsigned long b,int &y,unsigned long c) {
  if(b==0) { // ������� 
    x=c/a,y=0;
	return a;
  } else {
    int p=Extended_Euclid(b,x,a%b,y,c);
    unsigned long xTemp=x,yTemp=y;
    x=yTemp;
	  y=xTemp - a/b*yTemp; 
    return p;
  }
}

/*****************************����/���ܺ���*****************************/
/** 
 * ���ܺ���
 */
void Cipher() {
  string s;
  int tag;
  
  while(!infile.eof()) {
  	getline(infile, s);
  	cout<<s<<"\n";
    for (int i=0; i<s.length(); i++) {
      if ((s[i]!=' ') && (s[i]<'A' || (s[i]>'Z'&&s[i]<'a') || s[i]>'z'))
      	continue;
   	  tag = (s[i]==' ' ? 0:(fmod(s[i]-'A', 32)+1));
  	  unsigned long y = modN(tag, e, n);
      outfile<<y<<" ";
    }
  }
}

/** 
 * ���ܺ���
 */
void InvCipher() {
  char c;
  string s;   // �ļ��е����ִ� 
  int tag=0;  // ��ĸ��Ӧ�����
  infile>>s;
  for(int i=0; i<s.length(); i++) {
  	tag = tag*10 + (s[i]-'0');
  }
  while(infile) {
    // ���ܲ���
    unsigned long y = modN(tag, d, n);
    c = (y==0 ? ' ':(y-1+'A'));
    outfile<<c;

    // ��ȡ��һ���ֽ�
    infile>>s;
    tag = 0;
    for(int i=0; i<s.length(); i++) {
  	  tag = tag*10 + (s[i]-'0');
    }
  }
}

/**
 * ���ɼ���/������Կ��������س�ʼ������
 */
void Init() {
  initMiniPrime();
  
  // bug: ֻ����������������ͬ����...֮������Ծ�û���� 
  srand((unsigned)time(NULL));
  unsigned long p=0,q=0;
  while(p == q) {
  	p = Pseudoprime(8);
    q = Pseudoprime(9);
  }
  n = p*q;
  unsigned long  cn = (p-1)*(q-1);

  // ����һ����(p-1)*(q-1)���ص����� ��������Կ�� 
  srand((unsigned)time(NULL));
  while(TRUE) {
    e = randA2B(2, cn-1);
    if (is_probable_prime(e))
      break;
  }

  // ���ɽ�����Կ 
  int x, y;
  Extended_Euclid(cn, x, e, y, 1);
  d = ((y<0) ? y+cn:y);
  cout<<"================��Կ��Ϣ==================\n";
  cout<<"p = "<<p<<"\tq = "<<q
	  <<"\nn = "<<n
	  <<"\ne = "<<e<<"\td = "<<d;
  cout<<"\nInit Success!\n";
  cout<<"=========================================\n\n";
}

int main() {
  Init();

  char filename[100];

  // ���ܹ��̵���
  cout<<"����������ļ�����:";
  cin>>filename;
  infile.open(filename);
  outfile.open("CipherRetult.txt");
  Cipher();
  infile.close();
  outfile.close(); 
  
  // ���ܹ��̵���
  infile.open("CipherRetult.txt");
  outfile.open("InvResult.txt");
  InvCipher();
  infile.close();
  outfile.close();
  
  return 0;
}
