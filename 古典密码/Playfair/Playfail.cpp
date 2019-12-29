/****************************************************
* 													*
* ��Ŀ��     PlayFair�ŵ���������					*
* ���ߣ�     ������									*
* ���ʱ�䣺 2019.12.6								*
* 													*
****************************************************/ 
#include<iostream>
#include<string.h>
using namespace std;
// ������ܵ���󳤶� 
#define MAXLENGTH 1025

// ��������ַ�(Ϊȷ�������ִ�����Ϊż��) 
const char fillingLetter = 'q'; 

// ����Կ�������£� 
const char M[25] = {
	'c', 'i', 'p', 'h', 'e',
	'r', 'a', 'b', 'd', 'f',
	'g', 'k', 'l', 'm', 'n',
	'o', 'q', 's', 't', 'u',
	'v', 'w', 'x', 'y', 'z',
};

// �ж������ַ��Ƿ�Ϊ��ĸ 
int isLegitimate(char a) {
	int xiaoxie = (a>='a'&&a<='z');
	int daxie = (a>='A'&&a<='Z');
	if (xiaoxie || daxie) {
		return 1;
	} else {
		return 0;
	}
}

// �����Ľ���Ԥ����
// 1. ȥ���ո� 2. תСд�� 3. ���Ȼ�Ϊż�� 
void initString(char *m) {
	int i, j;
	for (i=0, j=0; j<strlen(m); i++, j++) {
		while(!isLegitimate(m[j]))
			j++;
		m[i] = m[j];
	}
	m[i] = '\0';
	
	int newLen = i-1;
	
	// ���ִ�ͳһתΪСд
	for (i=0; i<newLen; i++) {
		if (m[i]>'A' && m[i]<'Z') {
			m[i] = m[i] + 32;
		}
	}
	
	// ���ִ���ȫΪż������ 
	int len = newLen;
	if ((len%2) != 0) {
		m[len] = fillingLetter;
	}
}

// Ѱ�������ַ��� M �����е�λ�� 
int indexOf(char a) {
	for (int i=0; i<25; i++) {
		if (M[i] == a) {
			return i;
		}
	}
}


// *******���ܺ���******* 
// ���� m �����ܵ����ģ� c ���ܺ����� 
void myEncryption(char *m, char *c) {
	initString(m);
	
	int i;
	// ��ÿ����ĸ���м���
	for (i=0; i<strlen(m); i+=2) {
		int letter1 = indexOf(m[i]);
		int letter2 = indexOf(m[i+1]);
		
		int row1 = letter1/5;
		int row2 = letter2/5;
		int col1 = letter1%5;
		int col2 = letter2%5;
		
		if (row1 == row2) {        // ����ĸλ��ͬһ�� 
			if (col1 == 4)
				c[i] = M[row1*5];
			else 
				c[i] = M[letter1+1];
			
			if (col2 == 4)
				c[i+1] = M[row2*5];
			else 
				c[i+1] = M[letter2+1];
		} else if (col1 == col2) { // ����ĸλ��ͬһ�� 
			if (row1 == 4)
				c[i] = M[col1];
			else
				c[i] = M[letter1+5];
				
			if (row2 == 4)
				c[i+1] = M[col2];
			else
				c[i+1] = M[letter2+5];
		} else { 			       // �Ȳ�ͬһ�У�Ҳ����ͬһ�� 
			c[i] = M[letter1+(col2-col1)];
			c[i+1] = M[letter2-(col2-col1)];
		} // end if
	}
}

// *******���ܺ���******* 
void myDecrypt(char *c, char *m) {
	// ��ÿ����ĸ���м���
	for (int i=0; i<strlen(c); i+=2) {
		int letter1 = indexOf(c[i]);
		int letter2 = indexOf(c[i+1]);
		
		int row1 = letter1/5;
		int row2 = letter2/5;
		int col1 = letter1%5;
		int col2 = letter2%5;
		
		if (row1 == row2) {        // ����ĸλ��ͬһ�� 
			if (col1 == 0)
				m[i] = M[row1*5+4];
			else 
				m[i] = M[letter1-1];
			
			if (col2 == 0)
				m[i+1] = M[row2*5+4];
			else 
				m[i+1] = M[letter2-1];
		} else if (col1 == col2) { // ����ĸλ��ͬһ�� 
			if (row1 == 0)
				m[i] = M[col1+20];
			else
				m[i] = M[letter1-5];
				
			if (row2 == 0)
				m[i+1] = M[col2+20];
			else
				m[i+1] = M[letter2-5];
		} else { 			       // �Ȳ�ͬһ�У�Ҳ����ͬһ�� 
			m[i] = M[letter1+(col2-col1)];
			m[i+1] = M[letter2-(col2-col1)];
		} // end if
	} 
} 

int main() {
	// �����ַ��� 
	char m[MAXLENGTH];
	char c[MAXLENGTH];
	char am[MAXLENGTH];
	
	int i=0;
	// ��ȡ�����ֶ�, ʹ��getchar����Ϊ�����Զ���ո� 
//	cout<<"������Ҫ���ܵ����ģ�";
//	while((m[i] = getchar()) != '\n')
//		i++;
//	m[i+1] = '\0';
	
	
//	myEncryption(m, c);
//	myDecrypt(c, am);
//	cout<<"���ܺ�Ľ����"<<c<<"\n";
//	cout<<"���ܺ�Ľ����"<<am<<"\n";


	char a = 'a';
	a = a*3;
	cout<<a;
	return 0;
}










