#include<iostream>
#include<bitset>
#include<fstream>
#include<string.h> 
using namespace std;

typedef bitset<8> byte;
typedef bitset<32> word;

/* 输入文件和输出文件 */
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

// 逆S盒 
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

// Rcon常量 
word Rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 
				 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};


// 打印调试信息
void printGroup(byte b[16]) {
	int i=0;
	for (; i<16; i++) {
		cout<<b[i]<<" ";
		if(i%3 == 0)
			cout<<"\n";
	}
} 

// 把biset类型变量转int
int bit2Int(byte b) {
  b.to_ulong();
}

// 将字符串转为byte类型的4*4数组
byte str2Bytes(string str, byte w[4][4]) {
  int beg = 0;
  // 按列赋值，和书上二维数组结构相同
  for(int j=0; j<4; j++) {
    for (int i=0; i<4; i++) {
      byte temp(str.substr(beg*8, 8));
      w[i][j] = temp;
      beg++;
    }
  }
}

// SubBytes的辅助函数，将一个字节的前4bit赋给x，后4bit赋给y
void splitByte(byte b, int &x, int &y) {
  x=b[7]*8 + b[6]*4 + b[5]*2 + b[4];
  y=b[3]*8 + b[2]*4 + b[1]*2 + b[0];
}

// 单个字节变换，对应S盒
string SubSingleByte(byte &b) {
  int x, y;
  splitByte(b, x, y);
  cout<<x<<"   "<<y<<"\n";
  b = S_Box[x][y];
  cout<<"S_Box: "<<b<<"\n";
  return b.to_string();
}

// 单个字节变换，对应逆S盒
string InvSubSingleByte(byte &b) {
  int x, y;
  splitByte(b, x, y);
  b = Inv_S_Box[x][y];
  return b.to_string();
}


// 对字中的每一个字节进行subBytes
word SubWord(word w) {
  // 先将word转为string，并化分为一个个字节
  string str = w.to_string();
  byte b1(str, 0, 8);
  byte b2(str, 8, 8);
  byte b3(str, 16, 8);
  byte b4(str, 24, 8);
  
  // 只有0/1字符时，可以通过str直接给bitset赋值
  string targetStr = SubSingleByte(b1) + SubSingleByte(b2) + SubSingleByte(b3) + SubSingleByte(b4);
  word ret(targetStr);
  return ret;
}

// 对输入字的循环置换，左移一个字节
word RotWord(word w) {
  // 保存其高8位, 并将其转为int
  string str = w.to_string();
  byte temp(str, 0, 8);
  int last = bit2Int(temp);

  // 舍去高8位，并将其转为整型
  int pre = (w<<8).to_ulong();

  // 将原word循环左移一个字节，并将高8位或到最后
  word ret = (pre | last);
  return ret;
}

/* 密钥扩展算法 */
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

/* 全密钥加法变换 
* 传入扩展密钥的地址
* 传入此轮循环所需密钥的 起始地址
* 一次循环中，使用到的密钥长度为128
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
* 矩阵元素的乘法和加法都是定义在基于GF(2^8)上的二元运算,并不是通常意义上的乘法和加法。
* 这里涉及到一些信息安全上的数学知识，不过不懂这些知识也行。其实这种二元运算的加法等价于两个字节的异或
* 乘法则复杂一点。对于一个8位的二进制数来说，
** 乘2时，使用域上的乘法乘以(00000010)等价于左移1位(低位补0)后，再根据情况同(00011011)进行异或运算（当最高位为1时）
** 乘3时，乘以(0000 0011)可以拆分成先分别乘以(0000 0001)和(0000 0010)，再将两个乘积异或(已经过验证)
*/
int GFMul(int a, int b) {
  int temp;
  byte bTemp;
  switch(a) {
    case 2:
      temp = b;
      b = b<<1;
      if (temp/128 > 0)   // 判断b7是否为1
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
      // 如果不用byte对其高位进行截断，他可能会传入>0xff的整型，导致temp/128出错
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


// 密钥扩展
void KeyExpension(byte key[4*Nk], word w[4*(Nr+1)]) {
  word temp;
  int i=0;
  for(; i<Nk; i++) {
    word a = key[4*i].to_ulong();
    word b = key[4*i+1].to_ulong();
    word c = key[4*i+2].to_ulong();
    word d = key[4*i+3].to_ulong();
  	// w[i] = (a<<24) + (b<<16) + (c<<8) + d;  // 可能回越界吧...
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
// 加密函数相关函数 
/////////////////////////////////////////////////

/* 字节代替，非线性变换
这里通过S_Box实现
前两个字节对应的16进制数作为行号，后两个字节对应的16进制数作为列号
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
行移位
第0行不移位保持不变；第1行左移1个字节；第2行左移2个字节；第3行左移3个字节
*/
void ShiftRows(byte state[4][4]) {
  for (int i=1; i<4; i++) {
    byte temp[4] = state[i];
    for (int j=0; j<4; j++) {
      state[i][j] = temp[(j+i)%4];
    }
  }
};


/* 列混合变换 */
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
  // 第一次轮密钥加法运算
  AddRoundKey(state, w, 0);
  // 进行Nr-1次迭代
  for(int i=1; i<Nr; i++) {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(state, w, i*Nk);
  }
  // 最后一次不带MixColumns()
  SubBytes(state);
  ShiftRows(state);
  AddRoundKey(state, w, Nr*Nk);

  // 直接写入文件，就不进行转化了
  // out = state;
  for (int i=0; i<4; i++)
  	for (int j=0; j<4; j++)
	   outfile<<state[i][j];
}

/////////////////////////////////////////////////
// 解密函数相关函数 
/////////////////////////////////////////////////

/* 字节代替，非线性变换
* 这里通过S_Box实现
* 前两个字节对应的16进制数作为行号，
* 后两个字节对应的16进制数作为列号
*/
void InvSubBytes(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      InvSubSingleByte(state[i][j]);
    }
  }
};

/*
* 行移位
* 第0行不移位保持不变；第1行左移1个字节；第2行左移2个字节；第3行左移3个字节
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

/* 列混合变换 */
void InvMixColumns(byte state[4][4]) {
  for (int i=0; i<4; i++) {
    int s0=state[0][i].to_ulong();
    int s1=state[1][i].to_ulong();
    int s2=state[2][i].to_ulong();
    int s3=state[3][i].to_ulong();
    
    // 因为实现了矩阵 乘2，乘3的运算，所以大数运算靠小数加法实现
    // 例： 15·s0 = 8·s0 ^ 4·s0 ^ 2·s0
    // 又因为矩阵乘法特殊，**加法为异或** 上式 可以化简为 15*s0 = 3*s0
    // 可知： 0e = 1000^0100^0010 |  09 = 1000^0001  |  0d = 1000^0100  | 0b = 1000^0010^0001
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

  // 将结果输入到文件
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
  /* 读取密钥 */ 
  byte key[4];
  char cName[100];
  word w[4*(Nr+1)];
  ifstream inCipher;
  
  cout<<"请输入密钥文件名:";
  cin>>cName;
  inCipher.open(cName);
  
  char cText;
  inCipher>>cText;
  key[0] = cText;
  for(int i=1; i<4; i++) {
  	inCipher>>cText;
  	key[i] = cText;
  }
  
  /* 密钥扩展 */ 
  KeyExpension(key, w);

  /* 选择加密/解密操作 */ 
  byte in[16];

  int flag = 1;
  cout<<"1. 加密操作 | 2. 解密操作(1)：";
  cin>>flag;

  char filename[100];
  cout<<"请输入要加/解密的文件名：";
  cin>>filename;

  infile.open(filename);

  int i=0;
  if (flag == 1) {    // 加密操作
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
    // 当最后不足16字节时，直接放入加密 
    if (i%16 != 0)
      Cipher(in, w);
  } else if(flag == 2) {    // 解密操作
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
  cout<<"\n====== 完成 ======\n";
  return 0;
}


/*
1. 密钥扩展要看
*/