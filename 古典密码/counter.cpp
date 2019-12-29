#include<iostream>
#include<fstream> 
#include<string>
#include<ctype.h>
using namespace std;

int letters[95];
int totle = 0;

// ÅÐ¶ÏÃ÷ÎÄ×Ö·ûÊÇ·ñÎª×ÖÄ¸ 
int isLegitimate(char &a) {
	a = tolower(a);
	if (a>='a'&&a<='z') {
		return 1;
	} else {
		return 0;
	}
}

int main() {
	ifstream ifile;
	ifile.open("message.txt");
	
	string data;
	while(ifile>>data) {
		int len = data.size();
		for (int i=0; i<len; i++) {
			int no = data[i] - ' ';
			if (no>94 || no<0) continue;
				letters[no]++;
			totle++;
		} 
	}
	
	ifile.close();
	
	for (int i=0; i<95; i++) {
		char a = i + ' ';
		cout<<a<<":  "<<(float)letters[i]/totle<<"\n";
	}
	return 0;
} 
