#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
using namespace std; 
typedef bitset<8> byte;
typedef bitset<32> word;

const int Nk = 4;   // 密钥长度(以字为单位)
const int Nr = 10;  // 圈变换数目

byte S_Box[16][16] = {
	{0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
	{0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
	{0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
	{0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
	{0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
	{0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
	{0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
	{0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
	{0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
	{0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
	{0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
	{0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
	{0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
	{0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
	{0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
	{0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
};

byte Inv_S_Box[16][16] = {
	{0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
	{0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
	{0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
	{0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
	{0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
	{0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
	{0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
	{0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
	{0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
	{0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
	{0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
	{0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
	{0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
	{0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
	{0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
	{0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
};

// 轮常数，密钥扩展中用到。（AES-128只需要10轮）
word Rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 
				 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};

/******************************加密相关函数**********************/
/**
 * S盒变换 - 前4位为行号，后4位为列号
 */
void SubBytes(byte s[4*4]) {
	for(int i=0; i<16; ++i) {
		int row = s[i][7]*8 + s[i][6]*4 + s[i][5]*2 + s[i][4];
		int col = s[i][3]*8 + s[i][2]*4 + s[i][1]*2 + s[i][0];
		s[i] = S_Box[row][col];
	}
}

/**
 * 行变换 - 按字节循环左移
 * 第1行左移1个字节；第2行左移2个字节；第3行左移3个字节
 */
void ShiftRows(byte s[4*4]) {
	byte temp[16];  // 备份，方便移位赋值
	for (int i=0; i<16; i++)
		temp[i] = s[i];

	for (int i=1; i<4; i++) {
		int beg = i*4;
		for (int j=0; j<4; j++) {
			s[beg+j] = temp[beg+(i+j)%4];  // 左移
		}
	}
}

/**
 * 矩阵元素的乘法和加法都是定义在基于GF(2^8)上的二元运算,并不是通常意义上的乘法和加法。
 * 这里涉及到一些信息安全上的数学知识，不过不懂这些知识也行。其实这种二元运算的加法等价于两个字节的异或
 * 乘法则复杂一点。对于一个8位的二进制数来说，
 * 乘2时，使用域上的乘法乘以(00000010)等价于左移1位(低位补0)后，再根据情况同(00011011)进行异或运算（当最高位为1时）
 * 乘3时，乘以(0000 0011)可以拆分成先分别乘以(0000 0001)和(0000 0010)，再将两个乘积异或(已经过验证)
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

/**
 * 列变换
 * 主要调用GFMul函数
 */
void MixColumns(byte s[4*4]) {
	int arr[4];
	for(int i=0; i<4; ++i)
	{
		for(int j=0; j<4; ++j)
			arr[j] = s[i+j*4].to_ulong();

		s[i]    = GFMul(2, arr[0])^GFMul(3, arr[1])^arr[2]^arr[3];
		s[i+4]  = GFMul(2, arr[1])^GFMul(3, arr[2])^arr[0]^arr[3];
		s[i+8]  = GFMul(2, arr[2])^GFMul(3, arr[3])^arr[0]^arr[1];
		s[i+12] = GFMul(2, arr[3])^GFMul(3, arr[0])^arr[1]^arr[2];
	}
}

/**
 * 轮密钥加变换 - 将每一列与扩展密钥进行异或
 */
void AddRoundKey(byte s[4*4], word k[4])
{
	for(int i=0; i<4; ++i)
	{
		word k1 = k[i] >> 24;
		word k2 = (k[i] << 8) >> 24;
		word k3 = (k[i] << 16) >> 24;
		word k4 = (k[i] << 24) >> 24;
		
		s[i] = s[i] ^ byte(k1.to_ulong());
		s[i+4] = s[i+4] ^ byte(k2.to_ulong());
		s[i+8] = s[i+8] ^ byte(k3.to_ulong());
		s[i+12] = s[i+12] ^ byte(k4.to_ulong());
	}
}

/**************************解密相关函数***********************/
/**
 * 逆S盒变换
 */
void InvSubBytes(byte s[4*4]) {
	for(int i=0; i<16; ++i)
	{
		int row = s[i][7]*8 + s[i][6]*4 + s[i][5]*2 + s[i][4];
		int col = s[i][3]*8 + s[i][2]*4 + s[i][1]*2 + s[i][0];
		s[i] = Inv_S_Box[row][col];
	}
}

/**
 * 逆行变换 - 以字节为单位循环右移
 * 第0行不移位保持不变；第1行左移1个字节；第2行左移2个字节；第3行左移3个字节
 */
void InvShiftRows(byte s[4*4]) {
	byte temp[16];  // 备份，方便移位赋值
	for (int i=0; i<16; i++)
		temp[i] = s[i];

	for (int i=1; i<4; i++) {
		int beg = i*4;
		for (int j=0; j<4; j++) {
			int t=(j-i<0) ? (4+j-i):(j-i); // 右移
			s[beg+j] = temp[beg+t];
		}
	}
}

void InvMixColumns(byte s[4*4]) {
	int arr[4];
	for(int i=0; i<4; ++i)
	{
		for(int j=0; j<4; ++j)
			arr[j] = s[i+j*4].to_ulong();

		// 因为实现了矩阵 乘2，乘3的运算，所以大数运算靠小数加法实现
    // 例： 15·s0 = 8·s0 ^ 4·s0 ^ 2·s0
    // 又因为矩阵乘法特殊，**加法为异或** 上式 可以化简为 15*s0 = 3*s0
    // 可知： 0e = 1000^0100^0010 |  09 = 1000^0001  |  0d = 1000^0100^0001  | 0b = 1000^0010^0001
		s[i]    = (GFMul(8, arr[0])^GFMul(4, arr[0])^GFMul(2, arr[0])) ^ 
							(GFMul(8, arr[1])^GFMul(2, arr[1])^arr[1]) ^
							(GFMul(8, arr[2])^GFMul(4, arr[2])^arr[2]) ^
							(GFMul(8, arr[3])^arr[3]);
    s[4+i]  = (GFMul(8, arr[1])^GFMul(4, arr[1])^GFMul(2, arr[1])) ^
							(GFMul(8, arr[2])^GFMul(2, arr[2])^arr[2]) ^
							(GFMul(8, arr[3])^GFMul(4, arr[3])^arr[3]) ^
							(GFMul(8, arr[0])^arr[0]);
    s[8+i]  = (GFMul(8, arr[2])^GFMul(4, arr[2])^GFMul(2, arr[2])) ^
							(GFMul(8, arr[3])^GFMul(2, arr[3])^arr[3]) ^
							(GFMul(8, arr[0])^GFMul(4, arr[0])^arr[0]) ^
							(GFMul(8, arr[1])^arr[1]);
    s[12+i] = (GFMul(8, arr[3])^GFMul(4, arr[3])^GFMul(2, arr[3])) ^
							(GFMul(8, arr[0])^GFMul(2, arr[0])^arr[0]) ^
							(GFMul(8, arr[1])^GFMul(4, arr[1])^arr[1]) ^
							(GFMul(8, arr[2])^arr[2]);
	}
}

/******************************密钥扩展及相关函数***********************/
/**
 * 将4个 byte 转换为一个 word.
 */
word Word(byte& k1, byte& k2, byte& k3, byte& k4) {
	word result(0x00000000);
	word a = k1.to_ulong();
	word b = k2.to_ulong();
	word c = k3.to_ulong();
	word d = k4.to_ulong();
	result = (a<<=24) | (b<<=16) | (c<<=8) | d;
	return result;
}

/**
 *  对传入的字循环置换，左移一个字节
 */
word RotWord(word& w) {
	word high = w << 8;
	word low = w >> 24;
	return high | low;
}

/**
 * 对word中的每一个字节进行S-盒变换
 */
word SubWord(word w) {
	word temp;
	for(int i=0; i<32; i+=8)
	{
		int row = w[i+7]*8 + w[i+6]*4 + w[i+5]*2 + w[i+4];
		int col = w[i+3]*8 + w[i+2]*4 + w[i+1]*2 + w[i];
		byte val = S_Box[row][col];
		for(int j=0; j<8; ++j)
			temp[i+j] = val[j];
	}
	return temp;
}

/**
 *  密钥扩展函数
 */ 
void KeyExpansion(byte key[4*Nk], word w[4*(Nr+1)]) {
	word temp;
	int i = 0;
	for(; i<Nk; i++)
		w[i] = Word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);

	i = Nk;

	while(i < 4*(Nr+1)) {
		temp = w[i-1];
    if (i%Nk == 0)
      temp = SubWord(RotWord(temp)) ^ Rcon[i/Nk-1];
    else if ((Nk > 6) && (i%Nk == 4))
      temp = SubWord(temp);
    w[i] = w[i-Nk]^temp;
    i++;
	}
}

/******************************加密和解密函数**************************/
/**
 *  加密
 */
void Cipher(byte in[4*4], word w[4*(Nr+1)]) {
	word key[4];
	for(int i=0; i<4; ++i)
		key[i] = w[i];
	AddRoundKey(in, key);

	for(int round=1; round<Nr; ++round)
	{
		SubBytes(in);
		ShiftRows(in);
		MixColumns(in);
		for(int i=0; i<4; ++i)
			key[i] = w[4*round+i];
		AddRoundKey(in, key);
	}

	SubBytes(in);
	ShiftRows(in);
	for(int i=0; i<4; ++i)
		key[i] = w[4*Nr+i];
	AddRoundKey(in, key);
}

/**
 *  解密
 */
void InvCipher(byte in[4*4], word w[4*(Nr+1)]) {
	word key[4];
	for(int i=0; i<4; ++i)
		key[i] = w[4*Nr+i];
	AddRoundKey(in, key);

	for(int round=Nr-1; round>0; --round)
	{
		InvShiftRows(in);
		InvSubBytes(in);
		for(int i=0; i<4; ++i)
			key[i] = w[4*round+i];
		AddRoundKey(in, key);
		InvMixColumns(in);
	}

	InvShiftRows(in);
	InvSubBytes(in);
	for(int i=0; i<4; ++i)
		key[i] = w[i];
	AddRoundKey(in, key);
}

/******************************文件读写相关函数**************************/
/**
 *  将一个char字符数组转化为二进制
 *  存到一个 byte 数组中
 */
void charToByte(byte out[16], const char s[16]) { 
	for(int i=0; i<16; ++i)  
		for(int j=0; j<8; ++j)  
			out[i][j]= ((s[i]>>j) & 1);  
}

/**
 *  将连续的128位分成16组，存到一个 byte 数组中
 */
void divideToByte(byte out[16], bitset<128>& data) {
	bitset<128> temp;
	for(int i=0; i<16; ++i) {
		temp = (data << 8*i) >> 120;
		out[i] = temp.to_ulong();
	}
}

/**
 *  将16个 byte 合并成连续的128位
 */
bitset<128> mergeByte(byte in[16]) {
	bitset<128> res;
	res.reset();  // 置0
	bitset<128> temp;
	for(int i=0; i<16; ++i) {
		temp = in[i].to_ulong();
		temp <<= 8*(15-i);
		res |= temp;
	}
	return res;
}

int main() {
	string keyStr = "qwertyuiopasdfgh";
	byte key[16]; 
	charToByte(key, keyStr.c_str());
	// 密钥扩展
	word w[4*(Nr+1)];
	KeyExpansion(key, w);

	bitset<128> data;
	byte plain[16];
	// 加密
	ifstream infile;
	ofstream outfile;
	infile.open("message.txt", ios::binary);
	outfile.open("cipher.txt", ios::binary);
	while(infile.read((char*)&data, sizeof(data)))
	{
		divideToByte(plain, data);
		Cipher(plain, w);
		data = mergeByte(plain);
		outfile.write((char*)&data, sizeof(data));
		data.reset();  // 置0
	}
	infile.close();
	outfile.close();

	// 解密 
	infile.open("cipher.txt", ios::binary);
	outfile.open("InvCipher.txt", ios::binary);
	while(infile.read((char*)&data, sizeof(data)))
	{
		divideToByte(plain, data);
		InvCipher(plain, w);
		data = mergeByte(plain);
		outfile.write((char*)&data, sizeof(data));
		data.reset();  // 置0
	}
	infile.close();
	outfile.close();

	cout<<"=== 完成 ==="; 

	return 0;
}
