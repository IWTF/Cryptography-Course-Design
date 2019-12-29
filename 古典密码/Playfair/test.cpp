#include<iostream>
#include<string>
using namespace std;

string charToBits(char c)
{
	for(int i=0; i<8; i++) {
		cout<<((c>>i)&0x1);
	}
	cout<<"\n";
}

int main() {
	char a = 'a';
	char b = 'e';
	
	charToBits(a);
	charToBits(b);
	
	char c = a^b;
	charToBits(c);
	return 0;
}
