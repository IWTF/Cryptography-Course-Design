/****************************************************
* 													*
* 题目：     PlayFair古典密码体制					*
* 作者：     黄子文									*
* 完成时间： 2019.12.6								*
* 													*
****************************************************/ 
#include<iostream>
#include<string.h>
using namespace std;
// 定义加密的最大长度 
#define MAXLENGTH 1025

// 设置填充字符(为确保明文字串长度为偶数) 
const char fillingLetter = 'q'; 

// 设密钥矩阵如下： 
const char M[25] = {
	'c', 'i', 'p', 'h', 'e',
	'r', 'a', 'b', 'd', 'f',
	'g', 'k', 'l', 'm', 'n',
	'o', 'q', 's', 't', 'u',
	'v', 'w', 'x', 'y', 'z',
};

// 判断明文字符是否为字母 
int isLegitimate(char a) {
	int xiaoxie = (a>='a'&&a<='z');
	int daxie = (a>='A'&&a<='Z');
	if (xiaoxie || daxie) {
		return 1;
	} else {
		return 0;
	}
}

// 对明文进行预处理
// 1. 去除空格； 2. 转小写； 3. 长度化为偶数 
void initString(char *m) {
	int i, j;
	for (i=0, j=0; j<strlen(m); i++, j++) {
		while(!isLegitimate(m[j]))
			j++;
		m[i] = m[j];
	}
	m[i] = '\0';
	
	int newLen = i-1;
	
	// 将字串统一转为小写
	for (i=0; i<newLen; i++) {
		if (m[i]>'A' && m[i]<'Z') {
			m[i] = m[i] + 32;
		}
	}
	
	// 将字串补全为偶数长度 
	int len = newLen;
	if ((len%2) != 0) {
		m[len] = fillingLetter;
	}
}

// 寻找明文字符在 M 矩阵中的位置 
int indexOf(char a) {
	for (int i=0; i<25; i++) {
		if (M[i] == a) {
			return i;
		}
	}
}


// *******加密函数******* 
// 参数 m 待加密的明文； c 加密后密文 
void myEncryption(char *m, char *c) {
	initString(m);
	
	int i;
	// 对每组字母进行加密
	for (i=0; i<strlen(m); i+=2) {
		int letter1 = indexOf(m[i]);
		int letter2 = indexOf(m[i+1]);
		
		int row1 = letter1/5;
		int row2 = letter2/5;
		int col1 = letter1%5;
		int col2 = letter2%5;
		
		if (row1 == row2) {        // 两字母位于同一行 
			if (col1 == 4)
				c[i] = M[row1*5];
			else 
				c[i] = M[letter1+1];
			
			if (col2 == 4)
				c[i+1] = M[row2*5];
			else 
				c[i+1] = M[letter2+1];
		} else if (col1 == col2) { // 两字母位于同一列 
			if (row1 == 4)
				c[i] = M[col1];
			else
				c[i] = M[letter1+5];
				
			if (row2 == 4)
				c[i+1] = M[col2];
			else
				c[i+1] = M[letter2+5];
		} else { 			       // 既不同一行，也不是同一列 
			c[i] = M[letter1+(col2-col1)];
			c[i+1] = M[letter2-(col2-col1)];
		} // end if
	}
}

// *******解密函数******* 
void myDecrypt(char *c, char *m) {
	// 对每组字母进行加密
	for (int i=0; i<strlen(c); i+=2) {
		int letter1 = indexOf(c[i]);
		int letter2 = indexOf(c[i+1]);
		
		int row1 = letter1/5;
		int row2 = letter2/5;
		int col1 = letter1%5;
		int col2 = letter2%5;
		
		if (row1 == row2) {        // 两字母位于同一行 
			if (col1 == 0)
				m[i] = M[row1*5+4];
			else 
				m[i] = M[letter1-1];
			
			if (col2 == 0)
				m[i+1] = M[row2*5+4];
			else 
				m[i+1] = M[letter2-1];
		} else if (col1 == col2) { // 两字母位于同一列 
			if (row1 == 0)
				m[i] = M[col1+20];
			else
				m[i] = M[letter1-5];
				
			if (row2 == 0)
				m[i+1] = M[col2+20];
			else
				m[i+1] = M[letter2-5];
		} else { 			       // 既不同一行，也不是同一列 
			m[i] = M[letter1+(col2-col1)];
			m[i+1] = M[letter2-(col2-col1)];
		} // end if
	} 
} 

int main() {
	// 明文字符串 
	char m[MAXLENGTH];
	char c[MAXLENGTH];
	char am[MAXLENGTH];
	
	int i=0;
	// 获取明文字段, 使用getchar是因为它可以读入空格 
//	cout<<"请输入要加密的明文：";
//	while((m[i] = getchar()) != '\n')
//		i++;
//	m[i+1] = '\0';
	
	
//	myEncryption(m, c);
//	myDecrypt(c, am);
//	cout<<"加密后的结果："<<c<<"\n";
//	cout<<"解密后的结果："<<am<<"\n";


	char a = 'a';
	a = a*3;
	cout<<a;
	return 0;
}










