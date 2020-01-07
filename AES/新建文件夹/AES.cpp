#include<iostream>
#include<bitset>
#include<fstream>
#include<string.h> 
using namespace std;

typedef bitset<8> byte;
typedef bitset<32> word;

/* �����ļ�������ļ� */
ifstream infile;
ofstream outfile;

int const Nb = 4;
int const Nk = 4;
int const Nr = 10;

byte S_Box[16][16] = {
	{0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 
	 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
	{0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 
	 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
	{0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 
	 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
	{0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 
	 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
	{0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 
	 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
	{0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 
	 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
	{0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 
	 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
	{0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 
	 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
	{0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 
	 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
	{0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 
	 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
	{0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 
	 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
	{0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 
	 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
	{0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 
	 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
	{0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 
	 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
	{0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 
	 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
	{0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 
	 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
};

// ��S�� 
byte Inv_S_Box[16][16] = {
	{0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 
	 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
	{0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 
	 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
	{0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 
	 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
	{0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 
	 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
	{0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 
	 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
	{0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 
	 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
	{0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 
	 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
	{0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 
	 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
	{0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 
	 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
	{0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 
	 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
	{0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 
	 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
	{0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 
	 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
	{0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 
	 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
	{0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 
	 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
	{0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 
	 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
	{0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 
	 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
};

// Rcon���� 
word Rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 
				 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};


// ��ӡ������Ϣ
void printGroup(byte b[16]) {
	int i=0;
	for (; i<16; i++) {
		cout<<b[i]<<" ";
		if(i%3 == 0)
			cout<<"\n";
	}
} 

// ��biset���ͱ���תint
int bit2Int(byte b) {
  b.to_ulong();
}

// ���ַ���תΪbyte���͵�4*4����
byte str2Bytes(string str, byte w[4][4]) {
  int beg = 0;
  // ���и�ֵ�������϶�ά����ṹ��ͬ
  for(int j=0; j<4; j++) {
    for (int i=0; i<4; i++) {
      byte temp(str.substr(beg*8, 8));
      w[i][j] = temp;
      beg++;
    }
  }
}

// SubBytes�ĸ�����������һ���ֽڵ�ǰ4bit����x����4bit����y
void splitByte(byte b, int &x, int &y) {
  x=b[7]*8 + b[6]*4 + b[5]*2 + b[4];
  y=b[3]*8 + b[2]*4 + b[1]*2 + b[0];
}

// �����ֽڱ任����ӦS��
string SubSingleByte(byte &b) {
  int x, y;
  splitByte(b, x, y);
  cout<<x<<"   "<<y<<"\n";
  b = S_Box[x][y];
  cout<<"S_Box: "<<b<<"\n";
  return b.to_string();
}

// �����ֽڱ任����Ӧ��S��
string InvSubSingleByte(byte &b) {
  int x, y;
  splitByte(b, x, y);
  b = Inv_S_Box[x][y];
  return b.to_string();
}


// �����е�ÿһ���ֽڽ���subBytes
word SubWord(word w) {
  // �Ƚ�wordתΪstring��������Ϊһ�����ֽ�
  string str = w.to_string();
  byte b1(str, 0, 8);
  byte b2(str, 8, 8);
  byte b3(str, 16, 8);
  byte b4(str, 24, 8);
  
  // ֻ��0/1�ַ�ʱ������ͨ��strֱ�Ӹ�bitset��ֵ
  string targetStr = SubSingleByte(b1) + SubSingleByte(b2) + SubSingleByte(b3) + SubSingleByte(b4);
  word ret(targetStr);
  return ret;
}

// �������ֵ�ѭ���û�������һ���ֽ�
word RotWord(word w) {
  // �������8λ, ������תΪint
  string str = w.to_string();
  byte temp(str, 0, 8);
  int last = bit2Int(temp);

  // ��ȥ��8λ��������תΪ����
  int pre = (w<<8).to_ulong();

  // ��ԭwordѭ������һ���ֽڣ�������8λ�����
  word ret = (pre | last);
  return ret;
}

/* ��Կ��չ�㷨 */
void KeyExpansion(byte key[4*Nk], word w[4*(Nr+1)], int Nk) {
  word temp;
  int i=0;
  for (; i<Nk; i++)
    w[i] = bit2Int(key[4*i])<<3 + bit2Int(key[4*i+1])<<2 + bit2Int(key[4*i+2])<<1 + bit2Int(key[4*i+3]);
  while(i < 4*(Nr+1)) {
    temp = w[i-1];
    if (i%Nk == 0)
      temp = SubWord(RotWord(temp)) ^ Rcon[i/Nk];
    else if ((Nk > 6) && (i%Nk == 4))
      temp = SubWord(temp);
    w[i] = w[i-Nk]^temp;
    i += 1;
  }
}

/* ȫ��Կ�ӷ��任 
* ������չ��Կ�ĵ�ַ
* �������ѭ��������Կ�� ��ʼ��ַ
* һ��ѭ���У�ʹ�õ�����Կ����Ϊ128
*/
void AddRoundKey(byte state[4][4], word w[], int beg) {
  string str = w[beg].to_string() + w[beg+1].to_string() + w[beg+2].to_string() + w[beg+3].to_string();
  byte temp[4][4];
  str2Bytes(str, temp);
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      state[i][j] = state[i][j]^temp[i][j];
    }
  }
};

/*
* ����Ԫ�صĳ˷��ͼӷ����Ƕ����ڻ���GF(2^8)�ϵĶ�Ԫ����,������ͨ�������ϵĳ˷��ͼӷ���
* �����漰��һЩ��Ϣ��ȫ�ϵ���ѧ֪ʶ������������Щ֪ʶҲ�С���ʵ���ֶ�Ԫ����ļӷ��ȼ��������ֽڵ����
* �˷�����һ�㡣����һ��8λ�Ķ���������˵��
** ��2ʱ��ʹ�����ϵĳ˷�����(00000010)�ȼ�������1λ(��λ��0)���ٸ������ͬ(00011011)����������㣨�����λΪ1ʱ��
** ��3ʱ������(0000 0011)���Բ�ֳ��ȷֱ����(0000 0001)��(0000 0010)���ٽ������˻����(�Ѿ�����֤)
*/
int GFMul(int a, int b) {
  int temp;
  byte bTemp;
  switch(a) {
    case 2:
      temp = b;
      b = b<<1;
      if (temp/128 > 0)   // �ж�b7�Ƿ�Ϊ1
        b = b^0x1b;
      return b;
    case 3:
      temp = b;
      b = b<<1;
      if (temp/128 > 0)
        b = b^0x1b;
      b ^= temp;
      return b;
    case 4:
      // �������byte�����λ���нضϣ������ܻᴫ��>0xff�����ͣ�����temp/128����
      bTemp = GFMul(2, b);
      return GFMul(2, bTemp.to_ulong());
    case 8:
      bTemp = GFMul(4, b);
      return GFMul(2, bTemp.to_ulong());
    case 16:
      bTemp = GFMul(8, b);
      return GFMul(2, bTemp.to_ulong());
    default:
      return b;
  }
}


// ��Կ��չ
void KeyExpension(byte key[4*Nk], word w[4*(Nr+1)]) {
  word temp;
  int i=0;
  for(; i<Nk; i++) {
    word a = key[4*i].to_ulong();
    word b = key[4*i+1].to_ulong();
    word c = key[4*i+2].to_ulong();
    word d = key[4*i+3].to_ulong();
  	// w[i] = (a<<24) + (b<<16) + (c<<8) + d;  // ���ܻ�Խ���...
    w[i] = a^b^c^d;
	  i++;
  }
  i = Nk;
  while(i<4*(Nr+1)) {
    temp = w[i-1];
    if (i%Nk == 0)
      temp = SubWord(RotWord(temp)) ^ Rcon[i/Nk];
    else if (Nk>6 && (i%Nk == 4))
      temp = SubWord(temp);
    w[i] = w[i-Nk]^temp;
    i++;
  }
}

/////////////////////////////////////////////////
// ���ܺ�����غ��� 
/////////////////////////////////////////////////

/* �ֽڴ��棬�����Ա任
����ͨ��S_Boxʵ��
ǰ�����ֽڶ�Ӧ��16��������Ϊ�кţ��������ֽڶ�Ӧ��16��������Ϊ�к�
*/
void SubBytes(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      cout<<state[i][j]<<"\n";
      SubSingleByte(state[i][j]);
    }
  }
};


/*
����λ
��0�в���λ���ֲ��䣻��1������1���ֽڣ���2������2���ֽڣ���3������3���ֽ�
*/
void ShiftRows(byte state[4][4]) {
  for (int i=1; i<4; i++) {
    byte temp[4] = state[i];
    for (int j=0; j<4; j++) {
      state[i][j] = temp[(j+i)%4];
    }
  }
};


/* �л�ϱ任 */
void MixColumns(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    int s0=state[0][i].to_ulong();
    int s1=state[1][i].to_ulong();
    int s2=state[2][i].to_ulong();
    int s3=state[3][i].to_ulong();
    
    state[0][i] = GFMul(2, s0)^GFMul(3, s1)^s2^s3;
    state[1][i] = GFMul(2, s1)^GFMul(3, s2)^s2^s3;
    state[2][i] = GFMul(2, s2)^GFMul(3, s3)^s2^s3;
    state[3][i] = GFMul(2, s3)^GFMul(3, s0)^s2^s3;
  }
};

void Cipher(byte in[16], word w[Nk*(Nr+1)]) {
  byte state[4][4];
  string str;
  for (int i=0; i<16; i++)
  	str = str + in[i].to_string();
  str2Bytes(str, state);
  // ��һ������Կ�ӷ�����
  AddRoundKey(state, w, 0);
  // ����Nr-1�ε���
  for(int i=1; i<Nr; i++) {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(state, w, i*Nk);
  }
  // ���һ�β���MixColumns()
  SubBytes(state);
  ShiftRows(state);
  AddRoundKey(state, w, Nr*Nk);

  // ֱ��д���ļ����Ͳ�����ת����
  // out = state;
  for (int i=0; i<4; i++)
  	for (int j=0; j<4; j++)
	   outfile<<state[i][j];
}

/////////////////////////////////////////////////
// ���ܺ�����غ��� 
/////////////////////////////////////////////////

/* �ֽڴ��棬�����Ա任
* ����ͨ��S_Boxʵ��
* ǰ�����ֽڶ�Ӧ��16��������Ϊ�кţ�
* �������ֽڶ�Ӧ��16��������Ϊ�к�
*/
void InvSubBytes(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      InvSubSingleByte(state[i][j]);
    }
  }
};

/*
* ����λ
* ��0�в���λ���ֲ��䣻��1������1���ֽڣ���2������2���ֽڣ���3������3���ֽ�
*/
void InvShiftRows(byte state[4][4]) {
  for (int i=1; i<4; i++) {
    byte temp[4] = state[i];
    for (int j=0; j<4; j++) {
      int t=j-i;
      if ((j-i) < 0)
        t = 4+j-i;
      state[i][j] = temp[t];
    }
  }
};

/* �л�ϱ任 */
void InvMixColumns(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    int s0=state[0][i].to_ulong();
    int s1=state[1][i].to_ulong();
    int s2=state[2][i].to_ulong();
    int s3=state[3][i].to_ulong();
    
    // ��Ϊʵ���˾��� ��2����3�����㣬���Դ������㿿С���ӷ�ʵ��
    // ���� 15��s0 = 8��s0 ^ 4��s0 ^ 2��s0
    // ����Ϊ����˷����⣬**�ӷ�Ϊ���** ��ʽ ���Ի���Ϊ 15*s0 = 3*s0
    // ��֪�� 0e = 1000^0100^0010 |  09 = 1000^0001  |  0d = 1000^0100  | 0b = 1000^0010^0001
    state[0][i] = (GFMul(8, s0)^GFMul(4, s0)^GFMul(2, s0))^(GFMul(8, s1)^GFMul(2, s1)^s1)^(GFMul(8, s2)^GFMul(4, s2))^(GFMul(8, s3)^s3);
    state[1][i] = (GFMul(8, s1)^GFMul(4, s1)^GFMul(2, s1))^(GFMul(8, s2)^GFMul(2, s2)^s2)^(GFMul(8, s3)^GFMul(4, s3))^(GFMul(8, s0)^s0);
    state[2][i] = (GFMul(8, s2)^GFMul(4, s2)^GFMul(2, s2))^(GFMul(8, s3)^GFMul(2, s3)^s3)^(GFMul(8, s0)^GFMul(4, s0))^(GFMul(8, s1)^s1);
    state[3][i] = (GFMul(8, s3)^GFMul(4, s3)^GFMul(2, s3))^(GFMul(8, s0)^GFMul(2, s0)^s0)^(GFMul(8, s1)^GFMul(4, s1))^(GFMul(8, s2)^s2);
  }
}

void InvCipher(byte in[16], word w[Nk*(Nr+1)]) {
  byte state[4][4];
  string str;
  for (int i=0; i<16; i++)
  	str = str + in[i].to_string();

  str2Bytes(str, state);
  AddRoundKey(state, w, Nr*Nk);
  for (int i=1; i<Nr; i++) {
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, w, (Nr-i)*Nk);
    InvMixColumns(state);
  }
  InvShiftRows(state);
  InvSubBytes(state);
  AddRoundKey(state, w, 0);

  // ��������뵽�ļ�
  for (int i=0; i<4; i++) {
  	for (int j=0; j<4; j++) {
	  char a = (char)state[i][j].to_ulong();
	  cout<<a<<" ";
	  outfile<<a;
	}
	cout<<"\n";
  }
}

int main() {
  /* ��ȡ��Կ */ 
  byte key[4];
  char cName[100];
  word w[4*(Nr+1)];
  ifstream inCipher;
  
  cout<<"��������Կ�ļ���:";
  cin>>cName;
  inCipher.open(cName);
  
  char cText;
  inCipher>>cText;
  key[0] = cText;
  for(int i=1; i<4; i++) {
  	inCipher>>cText;
  	key[i] = cText;
  }
  
  /* ��Կ��չ */ 
  KeyExpension(key, w);

  /* ѡ�����/���ܲ��� */ 
  byte in[16];

  int flag = 1;
  cout<<"1. ���ܲ��� | 2. ���ܲ���(1)��";
  cin>>flag;

  char filename[100];
  cout<<"������Ҫ��/���ܵ��ļ�����";
  cin>>filename;

  infile.open(filename);

  int i=0;
  if (flag == 1) {    // ���ܲ���
    char data;
    outfile.open("Cipher.txt");

    infile>>data;
    while(infile) {
      in[i++] = data;
      infile>>data;
      if (i%16 == 0) {
        Cipher(in, w);
        i=0;
      }
    } // end while
    // �������16�ֽ�ʱ��ֱ�ӷ������ 
    if (i%16 != 0)
      Cipher(in, w);
  } else if(flag == 2) {    // ���ܲ���
    outfile.open("InvCipher.txt");

    infile>>in[i++];
    while(infile) {
      infile>>in[i++];
      if (i%16 == 0) {
        InvCipher(in, w);
        i=0;
      }
    } // end while
    if (i%16 != 0)
      InvCipher(in, w);
  } // end if

  infile.close();
  outfile.close();
  cout<<"\n====== ��� ======\n";
  return 0;
}


/*
1. ��Կ��չҪ��
*/