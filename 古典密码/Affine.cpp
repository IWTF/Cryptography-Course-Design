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

// ��˷���Ԫ 
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
	// Ϊ������Ĳ����ṩһ������ 
	srand((int)time(NULL));
	
	string data;
	ifstream  ifile;        // �����ļ�
	ifile.open(inputPath);
	
	ofstream ofile;         // ��������
	ofile.open(outputPath);
	ofstream k1file;        // ����k1
	k1file.open(k1Path);
	ofstream k2file;        // ����k2
	k2file.open(k2Path);
	
	
	// ��ȡ�ļ���������� 
	while(getline(ifile, data)) {
	//		cout<<"��ȡ��������Ϊ��"<<data<<"\n"; 
		int len = data.size();
		for (int i=0; i<len; i++) {
			int k1, k2;
			int m = data[i] - beginChar;
//			cout<<"m is:"<<m<<"\n";
						
			// ����������Կk1, k2(�ұ����q���ص������������г˷���) 
			k1 = random();
			while(inv(k1, q) == -1) {
				k1 = random();
			}
			k2 = random();
			while(inv(k2, q) == -1) {
				k2 = random();
			}
//			cout<<"��������Կk1, k2:"<<k1<<" "<<k2<<"\n"; 
			// ������Կ���Կո�ָ����������ʱ��ȡ��Կ 
			k1file<<k1<<" ";
			k2file<<k2<<" ";
			
			// ���ܲ��� (k1 + k2*m)modq
			char c = fmod((k1 + k2*m), q) + beginChar;
//			cout<<"c is:"<<c<<"\n";
			ofile<<c;
		}
		ofile<<endl;
	}
	
	// �رմ򿪵��ļ� 
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
	
	// ��ȡ�ļ���������� 
	while(getline(ifile, data)) {
//		cout<<"��ȡ��������Ϊ��"<<data<<" "<<data.size()<<"\n"; 
		int len = data.size();
		for (int i=0; i<len; i++) {
			int k1, k2;
			int m = data[i] - beginChar;
//			cout<<"=========================================\n";
//			cout<<"m is:"<<m<<"\n";
					
			k1file>>k1;
			k2file>>k2;
//			cout<<"key is:"<<k1<<" "<<k2<<"\n";
			
			// ���ܲ���
			int bc = fmod(((m-k1)*inv(k2, q)), q);
			bc = (bc+q)%q;	// ֱ��mod(q)ʱ���������ֵ.... 
			char c = bc + beginChar;
//			cout<<"c is:"<<bc<<"\t"<<c<<"\n";
//			cout<<"=========================================\n";
			ofile<<c;
		}
		ofile<<endl;
	}
	
	// �رմ򿪵��ļ� 
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
	
	// ���ܺ������� 
	myEncryption(inputPath, outputPath, k1Path, k2Path);
	
	// ���ܺ�������
	myDecrypt(outputPath, inputPath2, k1Path, k2Path); 
} 
