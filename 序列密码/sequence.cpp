/****************************************************
* 													*
* 题目：     序列密码								*
* 作者：     黄子文									*
* 完成时间： 2019.12.9								*
* 													*
****************************************************/ 
#include<iostream>
#include<math.h>
#include<fstream>
#include<string>
using namespace std;

const int n = 5;

// 求出下一状态
void nextState(int *arr)
{
	int a5 = arr[0]^arr[3];
	for (int i = 0; i < n - 1; i++)
	{
		arr[i] = arr[i + 1];
	}
	arr[n - 1] = a5;
}

// 由初始状态获得 输出序列seq； 
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

// 加密/解密函数 
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
	int s[32][5];     // 存储状态，总共有2^5共32种不同的状态
	int seq[32];      // 输出序列
	int s_num = 0;
 
	cout << "请输入初始状态:";
	for (int i = 0; i < n; i++)
		cin >> x[i];
	// 生成输出序列seq
	getState(x, s, seq); 

	char message[] = "message.txt";
	char ciphertext[] = "ciphertext.txt";
	char decrypttext[] = "decrypttext.txt";
	
	// 将message文件里的内容加密，存入ciphertext 
	myXOR(message, ciphertext, seq);
	
	// 将ciphertext里面的内容解密，存入decrypttext 
	myXOR(ciphertext, decrypttext, seq);
	return 0;
}
 
