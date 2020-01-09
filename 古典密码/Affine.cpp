#include<iostream>
#include<fstream>
#include<string>
#include <stdlib.h>
#include <time.h> 
#include<ctype.h>
#include<math.h>
using namespace std;

typedef long long ll;
const int q = 95;
const char beginChar = ' ';

#define random() (rand()%q)

// 求乘法逆元 
void exgcd(ll a,ll b,ll& d,ll& x,ll& y){
    if(!b) { d = a; x = 1; y = 0; }
    else{ exgcd(b, a%b, d, y, x); y -= x*(a/b); }
}
ll inv(ll a, ll p){
    ll d,x,y;
    exgcd(a,p,d,x,y);
    return d == 1 ? (x+p)%p : -1;
}

void myEncryption(char *inputPath, char *outputPath, char *k1Path, char *k2Path) {
	// 为随机数的产生提供一个种子 
	srand((int)time(NULL));
	
	string data;
	ifstream  ifile;        // 明文文件
	ifile.open(inputPath);
	
	ofstream ofile;         // 储存密文
	ofile.open(outputPath);
	ofstream k1file;        // 储存k1
	k1file.open(k1Path);
	ofstream k2file;        // 储存k2
	k2file.open(k2Path);
	
	
	// 读取文件，获得明文 
	while(getline(ifile, data)) {
	//		cout<<"读取到的数据为："<<data<<"\n"; 
		int len = data.size();
		for (int i=0; i<len; i++) {
			int k1, k2;
			int m = data[i] - beginChar;
//			cout<<"m is:"<<m<<"\n";
						
			// 产生加密密钥k1, k2(且必须和q互素的数，这样才有乘法逆) 
			k1 = random();
			while(inv(k1, q) == -1) {
				k1 = random();
			}
			k2 = random();
			while(inv(k2, q) == -1) {
				k2 = random();
			}
//			cout<<"产生的密钥k1, k2:"<<k1<<" "<<k2<<"\n"; 
			// 保存密钥；以空格分隔，方便解密时获取密钥 
			k1file<<k1<<" ";
			k2file<<k2<<" ";
			
			// 加密操作 (k1 + k2*m)modq
			char c = fmod((k1 + k2*m), q) + beginChar;
//			cout<<"c is:"<<c<<"\n";
			ofile<<c;
		}
		ofile<<endl;
	}
	
	// 关闭打开的文件 
	ifile.close();
	ofile.close();
	k1file.close();
	k2file.close();
}

void myDecrypt(char *inputPath, char *outputPath, char *k1Path, char *k2Path) {
	string data;
	
	ifstream  ifile;
	ifile.open(inputPath);
	ifstream k1file;
	k1file.open(k1Path);
	ifstream k2file;
	k2file.open(k2Path);
	
	ofstream ofile;
	ofile.open(outputPath);
	
	// 读取文件，获得明文 
	while(getline(ifile, data)) {
//		cout<<"读取到的数据为："<<data<<" "<<data.size()<<"\n"; 
		int len = data.size();
		for (int i=0; i<len; i++) {
			int k1, k2;
			int m = data[i] - beginChar;
//			cout<<"=========================================\n";
//			cout<<"m is:"<<m<<"\n";
					
			k1file>>k1;
			k2file>>k2;
//			cout<<"key is:"<<k1<<" "<<k2<<"\n";
			
			// 解密操作
			int bc = fmod(((m-k1)*inv(k2, q)), q);
			bc = (bc+q)%q;	// 直接mod(q)时，会出现正值.... 
			char c = bc + beginChar;
//			cout<<"c is:"<<bc<<"\t"<<c<<"\n";
//			cout<<"=========================================\n";
			ofile<<c;
		}
		ofile<<endl;
	}
	
	// 关闭打开的文件 
	ifile.close();
	ofile.close();
	k1file.close();
	k2file.close();
}

int main() {
	char inputPath[] = "message.txt";
	char inputPath2[] = "message2.txt";
	char outputPath[] = "ciphertext.txt";
	char k1Path[] = "k1.txt";
	char k2Path[] = "k2.txt";
	
	// 加密函数测试 
	myEncryption(inputPath, outputPath, k1Path, k2Path);
	
	// 解密函数测试
	myDecrypt(outputPath, inputPath2, k1Path, k2Path); 
} 
