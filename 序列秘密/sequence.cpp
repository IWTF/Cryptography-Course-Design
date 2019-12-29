/****************************************************
* 													*
* ��Ŀ��     ��������								*
* ���ߣ�     ������									*
* ���ʱ�䣺 2019.12.9								*
* 													*
****************************************************/ 
#include<iostream>
#include<math.h>
#include<fstream>
#include<string>
using namespace std;

const int n = 5;

// �����һ״̬
void nextState(int *arr)
{
	int a5 = arr[0]^arr[3];
	for (int i = 0; i < n - 1; i++)
	{
		arr[i] = arr[i + 1];
	}
	arr[n - 1] = a5;
}

// �ɳ�ʼ״̬��� �������seq�� 
void getState(int *initState, int s[32][5], int *seq){
	for (int i=0; i<32; i++) {
		seq[i] = initState[0];
		nextState(initState);
	}
}


char keyToChar(int beg, int *seq) {
	int ret=0;
	int no = 0;
	while(1) {
		ret = seq[beg++]*pow(2, no++);
		if (beg>31) beg = 0;
		if (no == 8) break;
	}
	return (char)ret;
} 

// ����/���ܺ��� 
void myXOR(char *iname, char *oname, int *seq) {
	ifstream ifile(iname);
	ofstream ofile(oname);
	
	int counter = 0;
	string msg;
	while(ifile>>msg) {
		int len = msg.size();
		char result[len];
		char key;
		for (int i=0; i<len; i++) {
			char key = keyToChar(counter, seq);
			result[i] = msg[i]^key;
			counter+=8;
		}
		ofile<<result;
	}
	
	ifile.close();
	ofile.close();
}
 
int main()
{
	int x[n];
	int s[32][5];     // �洢״̬���ܹ���2^5��32�ֲ�ͬ��״̬
	int seq[32];      // �������
	int s_num = 0;
 
	cout << "�������ʼ״̬:";
	for (int i = 0; i < n; i++)
		cin >> x[i];
	// �����������seq
	getState(x, s, seq); 

	char message[] = "message.txt";
	char ciphertext[] = "ciphertext.txt";
	char decrypttext[] = "decrypttext.txt";
	
	// ��message�ļ�������ݼ��ܣ�����ciphertext 
	myXOR(message, ciphertext, seq);
	
	// ��ciphertext��������ݽ��ܣ�����decrypttext 
	myXOR(ciphertext, decrypttext, seq);
	return 0;
}
 
