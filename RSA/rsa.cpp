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

unsigned long d = 147;  // 解密密钥
unsigned long e = 3;  // 加密密钥
unsigned long n = 253;  // n=pq

ifstream infile;
ofstream outfile;

#define randA2B(a, b) (rand() % (b-a+1))+ a;

/*****************************素性测试相关函数*****************************/
/**
 * 自定义取模函数，防止大数溢出 (a^q)mod(m)
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
 * 测试以a为底时，n是否为合数
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
 * Miller-Rabin素性测试
 * 要测试N是否为素数，首先将N-1分解为(2^s)d。
 * 在每次测试开始时，先随机选一个介于[1,N-1]的整数a，之后如果对所有的r∈[0,s-1]，
 * 若(a^d)modN≠1且(a^(2^r))*dmodN≠1，则N是合数。否则，N有3/4的概率为素数。
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

/*****************************大素数生成相关函数*****************************/
/**
 * 初始化生成一些小的素数，提高大素数产生效率
 */ 
void initMiniPrime() {
	for (int i=0; i<530; i++) {
		if (is_probable_prime(i, 10)) {
			miniPrime[num++] = i;
		}	
	}
}

/**
 * 产生一个长度为n的大素数,n最大为32
 */
unsigned long Pseudoprime(int n=15) {
  DWord temp;
  
  while (TRUE) {
    // 产生一个大奇数
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

/********************************求逆函数*******************************/
/**
 * 扩展欧几里得算法
 * xa+yb=c, 求x，y的值
 */
int Extended_Euclid(unsigned long a,int &x,unsigned long b,int &y,unsigned long c) {
  if(b==0) { // 特殊情况 
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

/*****************************加密/解密函数*****************************/
/** 
 * 加密函数
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
 * 解密函数
 */
void InvCipher() {
  char c;
  string s;   // 文件中的数字串 
  int tag=0;  // 字母对应的序号
  infile>>s;
  for(int i=0; i<s.length(); i++) {
  	tag = tag*10 + (s[i]-'0');
  }
  while(infile) {
    // 加密操作
    unsigned long y = modN(tag, d, n);
    c = (y==0 ? ' ':(y-1+'A'));
    outfile<<c;

    // 读取下一个字节
    infile>>s;
    tag = 0;
    for(int i=0; i<s.length(); i++) {
  	  tag = tag*10 + (s[i]-'0');
    }
  }
}

/**
 * 生成加密/解密密钥。进行相关初始化工作
 */
void Init() {
  initMiniPrime();
  
  // bug: 只能连续产生两个不同素数...之后随机性就没有了 
  srand((unsigned)time(NULL));
  unsigned long p=0,q=0;
  while(p == q) {
  	p = Pseudoprime(8);
    q = Pseudoprime(9);
  }
  n = p*q;
  unsigned long  cn = (p-1)*(q-1);

  // 产生一个与(p-1)*(q-1)互素的整数 （加密密钥） 
  srand((unsigned)time(NULL));
  while(TRUE) {
    e = randA2B(2, cn-1);
    if (is_probable_prime(e))
      break;
  }

  // 生成解密密钥 
  int x, y;
  Extended_Euclid(cn, x, e, y, 1);
  d = ((y<0) ? y+cn:y);
  cout<<"================密钥信息==================\n";
  cout<<"p = "<<p<<"\tq = "<<q
	  <<"\nn = "<<n
	  <<"\ne = "<<e<<"\td = "<<d;
  cout<<"\nInit Success!\n";
  cout<<"=========================================\n\n";
}

int main() {
  Init();

  char filename[100];

  // 机密过程调用
  cout<<"请输入加密文件名字:";
  cin>>filename;
  infile.open(filename);
  outfile.open("CipherRetult.txt");
  Cipher();
  infile.close();
  outfile.close(); 
  
  // 解密过程调用
  infile.open("CipherRetult.txt");
  outfile.open("InvResult.txt");
  InvCipher();
  infile.close();
  outfile.close();
  
  return 0;
}
