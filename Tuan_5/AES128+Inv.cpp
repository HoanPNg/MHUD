#include<iostream>
#include<iomanip>
#include<sstream>

using namespace std;

string IntToChar4(unsigned int I) {
	unsigned char c[4];

	c[0] = (I >> 24)&0xff;
	c[1] = (I >> 16)&0xff;
	c[2] = (I >> 8)&0xff;
	c[3] = I&0xff;

	string res(c, c+4);

	return res;
}

unsigned int Char16ToInts(string K, long pos) {
	unsigned int res = 0;
	
	unsigned int i1 = ((K[pos*4]) << 24) & 0xff000000;	
	unsigned int i2 = ((K[pos*4 + 1]) << 16) & 0x00ff0000;
	unsigned int i3 = ((K[pos*4 + 2]) << 8) & 0x0000ff00;
	unsigned int i4 = ((K[pos*4 + 3])) & 0x000000ff;

	res = i1 + i2 + i3 + i4;

	return res;
}

string IntsToChar16(unsigned int I1, unsigned int I2, unsigned int I3, unsigned int I4) {
	string C1 = IntToChar4(I1);
	string C2 = IntToChar4(I2);
	string C3 = IntToChar4(I3);
	string C4 = IntToChar4(I4);

	string res = C1 + C2 + C3 + C4;

	return res;
}

void PrintHex(string K) {
	for (int i = 0; i < 4; i++) {
		unsigned int temp = Char16ToInts(K, i);
		unsigned int c0 = ((temp >> 24) & 255);
		unsigned int c1 = ((temp >> 16) & 255);
		unsigned int c2 = ((temp >> 8) & 255);
		unsigned int c3 = (temp & 255);

		cout << hex << setw(2) << right << c0 << " " << setw(2) << right << c1 << " " << setw(2) << right << c2 << " " << setw(2) << right << c3 << " ";
	}
	cout << endl;
}

unsigned int AfterSBox(unsigned int KToSBox, unsigned char S_box[][16]) {
	//Lấy từng byte của int
	unsigned int b[4];
	for (int i = 0; i < 4; i++) {
		b[3 - i] = (KToSBox >> (8 * i)) & 255;
	}

	//Lấy mỗi 4 bit của những byte vừa lấy được để tra S box
	unsigned int bS[4];
	for (int i = 0; i < 4; i++) {
		int x = ((b[i]) >> 4) & 15;
		int y = b[i] & 15;
		bS[i] = S_box[x][y];
	}

	//Ghép những byte vừa lấy từ S box lại thành int
	unsigned int res = 0;
	for (int i = 0; i < 4; i++) {
		res += (bS[3-i] << (8 * i));
	}

	return res;
}

void ExpandK(string K, unsigned int C[], unsigned char S_box[][16], string KRes[]) {
	KRes[0] = K;

	string PreK = K;
	for (int i = 1; i < 11; i++) {
		unsigned int PreK0 = Char16ToInts(PreK, 0);
		unsigned int PreK1 = Char16ToInts(PreK, 1);
		unsigned int PreK2 = Char16ToInts(PreK, 2);
		unsigned int PreK3 = Char16ToInts(PreK, 3);

		unsigned int KToSBox = (PreK3 << 8) | (PreK3 >> 24);
		unsigned int afterS = AfterSBox(KToSBox, S_box);

		unsigned int CrrK0 = PreK0 ^ afterS ^ C[i];
		unsigned int CrrK1 = PreK1 ^ CrrK0;
		unsigned int CrrK2 = PreK2 ^ CrrK1;
		unsigned int CrrK3 = PreK3 ^ CrrK2;

		string CrrK = IntsToChar16(CrrK0, CrrK1, CrrK2, CrrK3);

		KRes[i] = CrrK;
		PreK = CrrK;
	}
}

string XorChar16(string A, string B) {
	unsigned int Ai[4];
	for (int i = 0; i < 4; i++) {
		Ai[i] = Char16ToInts(A, i);
	}

	unsigned int Bi[4];
	for (int i = 0; i < 4; i++) {
		Bi[i] = Char16ToInts(B, i);
	}

	unsigned int Resi[4];
	for (int i = 0; i < 4; i++) {
		Resi[i] = Ai[i] ^ Bi[i];
	}

	return IntsToChar16(Resi[0], Resi[1], Resi[2], Resi[3]);
}

string SBox16(string s, unsigned char S_box[][16]) {
	unsigned int tempInt[4];

	for (int i = 0; i < 4; i++) {
		tempInt[i] = Char16ToInts(s, i);
	}

	for (int i = 0; i < 4; i++) {
		tempInt[i] = AfterSBox(tempInt[i], S_box);
	}

	return IntsToChar16(tempInt[0], tempInt[1], tempInt[2], tempInt[3]);
}

string ShiftRows(string s) {
	stringstream buffer;

	buffer << s[0] << s[5] << s[10] << s[15] << s[4] << s[9] << s[14]
		<< s[3] << s[8] << s[13] << s[2] << s[7] << s[12] << s[1] << s[6] << s[11];
	
	return buffer.str();
}

unsigned char NhanMBox(unsigned char s, unsigned char m) {
	if (m == 0x01) {
		return s;
	}
	if (m == 0x02) {
		unsigned char msb = (s >> 7)&0x01;
		s = s << 1;
		if (msb == 1) {
			s = s ^ 0x1b;
		}
		return s;
	}
	unsigned temp = s;
	unsigned char msb = (s >> 7) & 0x01;
	s = s << 1;
	if (msb == 1) {
		s = s ^ 0x1b;
	}
	s = s ^ temp;
	return s;
}

string MixColumns(string s, unsigned char M_box[][4]) {
	string res = "";
	for (int i = 0; i < 4; i++) {
		// Lấy 4 byte (1 cột)
		unsigned int col = Char16ToInts(s, i);

		unsigned char s0 = (col >> 24)&0xff;
		unsigned char s1 = (col >> 16) & 0xff;
		unsigned char s2 = (col >> 8) & 0xff;
		unsigned char s3 = (col) & 0xff;

		unsigned char c[4];

		// Lấy từng byte nhân với bảng m box
		c[0] = NhanMBox(s0, M_box[0][0]) ^ NhanMBox(s1, M_box[0][1]) ^ NhanMBox(s2, M_box[0][2]) ^ NhanMBox(s3, M_box[0][3]);
		c[1] = NhanMBox(s0, M_box[1][0]) ^ NhanMBox(s1, M_box[1][1]) ^ NhanMBox(s2, M_box[1][2]) ^ NhanMBox(s3, M_box[1][3]);
		c[2] = NhanMBox(s0, M_box[2][0]) ^ NhanMBox(s1, M_box[2][1]) ^ NhanMBox(s2, M_box[2][2]) ^ NhanMBox(s3, M_box[2][3]);
		c[3] = NhanMBox(s0, M_box[3][0]) ^ NhanMBox(s1, M_box[3][1]) ^ NhanMBox(s2, M_box[3][2]) ^ NhanMBox(s3, M_box[3][3]);

		// Đổi 4 byte vừa qua bảng m box lại thành string
		string temp(c, c + 4);

		res += temp;
	}

	return res;
}

string AES128(string K, string M, unsigned int C[], unsigned char S_box[][16], unsigned char M_box[][4]) {
	string KRes[11];
	for (int i = 0; i < 11; i++) {
		KRes[i] = "";
	}

	ExpandK(K, C, S_box, KRes);

	string s = XorChar16(M, KRes[0]);
	cout << "S0 : ";
	PrintHex(s);

	for (int i = 1; i < 11; i++) {
		s = SBox16(s, S_box);
		s = ShiftRows(s);
		if (i <= 9) {
			s = MixColumns(s, M_box);
		}
		s = XorChar16(s, KRes[i]);

		cout << "S" << setw(2) << left << dec << i << ": ";
		PrintHex(s);
	}

	return s;
}

string InvShiftRows(string s) {
	stringstream buffer;

	buffer << s[0] << s[13] << s[10] << s[7] << s[4] << s[1] << s[14]
		<< s[11] << s[8] << s[5] << s[2] << s[15] << s[12] << s[9] << s[6] << s[3];

	return buffer.str();
}

unsigned char NhanInvMBox(unsigned char s, unsigned char m) {
	if (m == 0x0e) {
		return NhanMBox(NhanMBox(NhanMBox(s, 0x02), 0x02), 0x02) ^ NhanMBox(NhanMBox(s, 0x02), 0x02) ^ NhanMBox(s, 0x02);
	}
	if (m == 0x0b) {
		return NhanMBox(NhanMBox(NhanMBox(s, 0x02), 0x02), 0x02) ^ NhanMBox(s, 0x02) ^ s;
	}
	if (m == 0x0d) {
		return NhanMBox(NhanMBox(NhanMBox(s, 0x02), 0x02), 0x02) ^ NhanMBox(NhanMBox(s, 0x02), 0x02) ^ s;
	}
	return NhanMBox(NhanMBox(NhanMBox(s, 0x02), 0x02), 0x02) ^ s;
}

string InvMixColumns(string s, unsigned char Inv_M_box[][4]) {
	string res = "";
	for (int i = 0; i < 4; i++) {
		// Lấy 4 byte (1 cột)
		unsigned int col = Char16ToInts(s, i);

		unsigned char s0 = (col >> 24) & 0xff;
		unsigned char s1 = (col >> 16) & 0xff;
		unsigned char s2 = (col >> 8) & 0xff;
		unsigned char s3 = (col) & 0xff;

		unsigned char c[4];

		// Lấy từng byte nhân với bảng m box
		c[0] = NhanInvMBox(s0, Inv_M_box[0][0]) ^ NhanInvMBox(s1, Inv_M_box[0][1]) ^ NhanInvMBox(s2, Inv_M_box[0][2]) ^ NhanInvMBox(s3, Inv_M_box[0][3]);
		c[1] = NhanInvMBox(s0, Inv_M_box[1][0]) ^ NhanInvMBox(s1, Inv_M_box[1][1]) ^ NhanInvMBox(s2, Inv_M_box[1][2]) ^ NhanInvMBox(s3, Inv_M_box[1][3]);
		c[2] = NhanInvMBox(s0, Inv_M_box[2][0]) ^ NhanInvMBox(s1, Inv_M_box[2][1]) ^ NhanInvMBox(s2, Inv_M_box[2][2]) ^ NhanInvMBox(s3, Inv_M_box[2][3]);
		c[3] = NhanInvMBox(s0, Inv_M_box[3][0]) ^ NhanInvMBox(s1, Inv_M_box[3][1]) ^ NhanInvMBox(s2, Inv_M_box[3][2]) ^ NhanInvMBox(s3, Inv_M_box[3][3]);

		// Đổi 4 byte vừa qua bảng m box lại thành string
		string temp(c, c + 4);

		res += temp;
	}

	return res;
}

string InvAES128(string KRes[], string s, unsigned char Inv_S_box[][16], unsigned char Inv_M_box[][4]) {
	for (int i = 10; i > 0; i--) {
		s = XorChar16(s, KRes[i]);
		if (i < 10) {
			s = InvMixColumns(s, Inv_M_box);
		}
		s = InvShiftRows(s);
		s = SBox16(s, Inv_S_box);
	}
	s = XorChar16(s, KRes[0]);

	return s;
}

int main() {
	unsigned char S_box[16][16];
	S_box[0][0] = 0x63;
	S_box[1][0] = 0xca;
	S_box[2][0] = 0xb7;
	S_box[3][0] = 0x04;
	S_box[4][0] = 0x09;
	S_box[5][0] = 0x53;
	S_box[6][0] = 0xd0;
	S_box[7][0] = 0x51;
	S_box[8][0] = 0xcd;
	S_box[9][0] = 0x60;
	S_box[10][0] = 0xe0;
	S_box[11][0] = 0xe7;
	S_box[12][0] = 0xba;
	S_box[13][0] = 0x70;
	S_box[14][0] = 0xe1;
	S_box[15][0] = 0x8c;
	S_box[0][1] = 0x7c;
	S_box[1][1] = 0x82;
	S_box[2][1] = 0xfd;
	S_box[3][1] = 0xc7;
	S_box[4][1] = 0x83;
	S_box[5][1] = 0xd1;
	S_box[6][1] = 0xef;
	S_box[7][1] = 0xa3;
	S_box[8][1] = 0x0c;
	S_box[9][1] = 0x81;
	S_box[10][1] = 0x32;
	S_box[11][1] = 0xc8;
	S_box[12][1] = 0x78;
	S_box[13][1] = 0x3e;
	S_box[14][1] = 0xf8;
	S_box[15][1] = 0xa1;
	S_box[0][2] = 0x77;
	S_box[1][2] = 0xc9;
	S_box[2][2] = 0x93;
	S_box[3][2] = 0x23;
	S_box[4][2] = 0x2c;
	S_box[5][2] = 0x00;
	S_box[6][2] = 0xaa;
	S_box[7][2] = 0x40;
	S_box[8][2] = 0x13;
	S_box[9][2] = 0x4f;
	S_box[10][2] = 0x3a;
	S_box[11][2] = 0x37;
	S_box[12][2] = 0x25;
	S_box[13][2] = 0xb5;
	S_box[14][2] = 0x98;
	S_box[15][2] = 0x89;
	S_box[0][3] = 0x7b;
	S_box[1][3] = 0x7d;
	S_box[2][3] = 0x26;
	S_box[3][3] = 0xc3;
	S_box[4][3] = 0x1a;
	S_box[5][3] = 0xed;
	S_box[6][3] = 0xfb;
	S_box[7][3] = 0x8f;
	S_box[8][3] = 0xec;
	S_box[9][3] = 0xdc;
	S_box[10][3] = 0x0a;
	S_box[11][3] = 0x6d;
	S_box[12][3] = 0x2e;
	S_box[13][3] = 0x66;
	S_box[14][3] = 0x11;
	S_box[15][3] = 0x0d;
	S_box[0][4] = 0xf2;
	S_box[1][4] = 0xfa;
	S_box[2][4] = 0x36;
	S_box[3][4] = 0x18;
	S_box[4][4] = 0x1b;
	S_box[5][4] = 0x20;
	S_box[6][4] = 0x43;
	S_box[7][4] = 0x92;
	S_box[8][4] = 0x5f;
	S_box[9][4] = 0x22;
	S_box[10][4] = 0x49;
	S_box[11][4] = 0x8d;
	S_box[12][4] = 0x1c;
	S_box[13][4] = 0x48;
	S_box[14][4] = 0x69;
	S_box[15][4] = 0xbf;
	S_box[0][5] = 0x6b;
	S_box[1][5] = 0x59;
	S_box[2][5] = 0x3f;
	S_box[3][5] = 0x96;
	S_box[4][5] = 0x6e;
	S_box[5][5] = 0xfc;
	S_box[6][5] = 0x4d;
	S_box[7][5] = 0x9d;
	S_box[8][5] = 0x97;
	S_box[9][5] = 0x2a;
	S_box[10][5] = 0x06;
	S_box[11][5] = 0xd5;
	S_box[12][5] = 0xa6;
	S_box[13][5] = 0x03;
	S_box[14][5] = 0xd9;
	S_box[15][5] = 0xe6;
	S_box[0][6] = 0x6f;
	S_box[1][6] = 0x47;
	S_box[2][6] = 0xf7;
	S_box[3][6] = 0x05;
	S_box[4][6] = 0x5a;
	S_box[5][6] = 0xb1;
	S_box[6][6] = 0x33;
	S_box[7][6] = 0x38;
	S_box[8][6] = 0x44;
	S_box[9][6] = 0x90;
	S_box[10][6] = 0x24;
	S_box[11][6] = 0x4e;
	S_box[12][6] = 0xb4;
	S_box[13][6] = 0xf6;
	S_box[14][6] = 0x8e;
	S_box[15][6] = 0x42;
	S_box[0][7] = 0xc5;
	S_box[1][7] = 0xf0;
	S_box[2][7] = 0xcc;
	S_box[3][7] = 0x9a;
	S_box[4][7] = 0xa0;
	S_box[5][7] = 0x5b;
	S_box[6][7] = 0x85;
	S_box[7][7] = 0xf5;
	S_box[8][7] = 0x17;
	S_box[9][7] = 0x88;
	S_box[10][7] = 0x5c;
	S_box[11][7] = 0xa9;
	S_box[12][7] = 0xc6;
	S_box[13][7] = 0x0e;
	S_box[14][7] = 0x94;
	S_box[15][7] = 0x68;
	S_box[0][8] = 0x30;
	S_box[1][8] = 0xad;
	S_box[2][8] = 0x34;
	S_box[3][8] = 0x07;
	S_box[4][8] = 0x52;
	S_box[5][8] = 0x6a;
	S_box[6][8] = 0x45;
	S_box[7][8] = 0xbc;
	S_box[8][8] = 0xc4;
	S_box[9][8] = 0x46;
	S_box[10][8] = 0xc2;
	S_box[11][8] = 0x6c;
	S_box[12][8] = 0xe8;
	S_box[13][8] = 0x61;
	S_box[14][8] = 0x9b;
	S_box[15][8] = 0x41;
	S_box[0][9] = 0x01;
	S_box[1][9] = 0xd4;
	S_box[2][9] = 0xa5;
	S_box[3][9] = 0x12;
	S_box[4][9] = 0x3b;
	S_box[5][9] = 0xcb;
	S_box[6][9] = 0xf9;
	S_box[7][9] = 0xb6;
	S_box[8][9] = 0xa7;
	S_box[9][9] = 0xee;
	S_box[10][9] = 0xd3;
	S_box[11][9] = 0x56;
	S_box[12][9] = 0xdd;
	S_box[13][9] = 0x35;
	S_box[14][9] = 0x1e;
	S_box[15][9] = 0x99;
	S_box[0][10] = 0x67;
	S_box[1][10] = 0xa2;
	S_box[2][10] = 0xe5;
	S_box[3][10] = 0x80;
	S_box[4][10] = 0xd6;
	S_box[5][10] = 0xbe;
	S_box[6][10] = 0x02;
	S_box[7][10] = 0xda;
	S_box[8][10] = 0x7e;
	S_box[9][10] = 0xb8;
	S_box[10][10] = 0xac;
	S_box[11][10] = 0xf4;
	S_box[12][10] = 0x74;
	S_box[13][10] = 0x57;
	S_box[14][10] = 0x87;
	S_box[15][10] = 0x2d;
	S_box[0][11] = 0x2b;
	S_box[1][11] = 0xaf;
	S_box[2][11] = 0xf1;
	S_box[3][11] = 0xe2;
	S_box[4][11] = 0xb3;
	S_box[5][11] = 0x39;
	S_box[6][11] = 0x7f;
	S_box[7][11] = 0x21;
	S_box[8][11] = 0x3d;
	S_box[9][11] = 0x14;
	S_box[10][11] = 0x62;
	S_box[11][11] = 0xea;
	S_box[12][11] = 0x1f;
	S_box[13][11] = 0xb9;
	S_box[14][11] = 0xe9;
	S_box[15][11] = 0x0f;
	S_box[0][12] = 0xfe;
	S_box[1][12] = 0x9c;
	S_box[2][12] = 0x71;
	S_box[3][12] = 0xeb;
	S_box[4][12] = 0x29;
	S_box[5][12] = 0x4a;
	S_box[6][12] = 0x50;
	S_box[7][12] = 0x10;
	S_box[8][12] = 0x64;
	S_box[9][12] = 0xde;
	S_box[10][12] = 0x91;
	S_box[11][12] = 0x65;
	S_box[12][12] = 0x4b;
	S_box[13][12] = 0x86;
	S_box[14][12] = 0xce;
	S_box[15][12] = 0xb0;
	S_box[0][13] = 0xd7;
	S_box[1][13] = 0xa4;
	S_box[2][13] = 0xd8;
	S_box[3][13] = 0x27;
	S_box[4][13] = 0xe3;
	S_box[5][13] = 0x4c;
	S_box[6][13] = 0x3c;
	S_box[7][13] = 0xff;
	S_box[8][13] = 0x5d;
	S_box[9][13] = 0x5e;
	S_box[10][13] = 0x95;
	S_box[11][13] = 0x7a;
	S_box[12][13] = 0xbd;
	S_box[13][13] = 0xc1;
	S_box[14][13] = 0x55;
	S_box[15][13] = 0x54;
	S_box[0][14] = 0xab;
	S_box[1][14] = 0x72;
	S_box[2][14] = 0x31;
	S_box[3][14] = 0xb2;
	S_box[4][14] = 0x2f;
	S_box[5][14] = 0x58;
	S_box[6][14] = 0x9f;
	S_box[7][14] = 0xf3;
	S_box[8][14] = 0x19;
	S_box[9][14] = 0x0b;
	S_box[10][14] = 0xe4;
	S_box[11][14] = 0xae;
	S_box[12][14] = 0x8b;
	S_box[13][14] = 0x1d;
	S_box[14][14] = 0x28;
	S_box[15][14] = 0xbb;
	S_box[0][15] = 0x76;
	S_box[1][15] = 0xc0;
	S_box[2][15] = 0x15;
	S_box[3][15] = 0x75;
	S_box[4][15] = 0x84;
	S_box[5][15] = 0xcf;
	S_box[6][15] = 0xa8;
	S_box[7][15] = 0xd2;
	S_box[8][15] = 0x73;
	S_box[9][15] = 0xdb;
	S_box[10][15] = 0x79;
	S_box[11][15] = 0x08;
	S_box[12][15] = 0x8a;
	S_box[13][15] = 0x9e;
	S_box[14][15] = 0xdf;
	S_box[15][15] = 0x16;

	unsigned char Inv_S_box[16][16];
	Inv_S_box[0][0] = 0x52;
	Inv_S_box[1][0] = 0x7c;
	Inv_S_box[2][0] = 0x54;
	Inv_S_box[3][0] = 0x08;
	Inv_S_box[4][0] = 0x72;
	Inv_S_box[5][0] = 0x6c;
	Inv_S_box[6][0] = 0x90;
	Inv_S_box[7][0] = 0xd0;
	Inv_S_box[8][0] = 0x3a;
	Inv_S_box[9][0] = 0x96;
	Inv_S_box[10][0] = 0x47;
	Inv_S_box[11][0] = 0xfc;
	Inv_S_box[12][0] = 0x1f;
	Inv_S_box[13][0] = 0x60;
	Inv_S_box[14][0] = 0xa0;
	Inv_S_box[15][0] = 0x17;
	Inv_S_box[0][1] = 0x09;
	Inv_S_box[1][1] = 0xe3;
	Inv_S_box[2][1] = 0x7b;
	Inv_S_box[3][1] = 0x2e;
	Inv_S_box[4][1] = 0xf8;
	Inv_S_box[5][1] = 0x70;
	Inv_S_box[6][1] = 0xd8;
	Inv_S_box[7][1] = 0x2c;
	Inv_S_box[8][1] = 0x91;
	Inv_S_box[9][1] = 0xac;
	Inv_S_box[10][1] = 0xf1;
	Inv_S_box[11][1] = 0x56;
	Inv_S_box[12][1] = 0xdd;
	Inv_S_box[13][1] = 0x51;
	Inv_S_box[14][1] = 0xe0;
	Inv_S_box[15][1] = 0x2b;
	Inv_S_box[0][2] = 0x6a;
	Inv_S_box[1][2] = 0x39;
	Inv_S_box[2][2] = 0x94;
	Inv_S_box[3][2] = 0xa1;
	Inv_S_box[4][2] = 0xf6;
	Inv_S_box[5][2] = 0x48;
	Inv_S_box[6][2] = 0xab;
	Inv_S_box[7][2] = 0x1e;
	Inv_S_box[8][2] = 0x11;
	Inv_S_box[9][2] = 0x74;
	Inv_S_box[10][2] = 0x1a;
	Inv_S_box[11][2] = 0x3e;
	Inv_S_box[12][2] = 0xa8;
	Inv_S_box[13][2] = 0x7f;
	Inv_S_box[14][2] = 0x3b;
	Inv_S_box[15][2] = 0x04;
	Inv_S_box[0][3] = 0xd5;
	Inv_S_box[1][3] = 0x82;
	Inv_S_box[2][3] = 0x32;
	Inv_S_box[3][3] = 0x66;
	Inv_S_box[4][3] = 0x64;
	Inv_S_box[5][3] = 0x50;
	Inv_S_box[6][3] = 0x00;
	Inv_S_box[7][3] = 0x8f;
	Inv_S_box[8][3] = 0x41;
	Inv_S_box[9][3] = 0x22;
	Inv_S_box[10][3] = 0x71;
	Inv_S_box[11][3] = 0x4b;
	Inv_S_box[12][3] = 0x33;
	Inv_S_box[13][3] = 0xa9;
	Inv_S_box[14][3] = 0x4d;
	Inv_S_box[15][3] = 0x7e;
	Inv_S_box[0][4] = 0x30;
	Inv_S_box[1][4] = 0x9b;
	Inv_S_box[2][4] = 0xa6;
	Inv_S_box[3][4] = 0x28;
	Inv_S_box[4][4] = 0x86;
	Inv_S_box[5][4] = 0xfd;
	Inv_S_box[6][4] = 0x8c;
	Inv_S_box[7][4] = 0xca;
	Inv_S_box[8][4] = 0x4f;
	Inv_S_box[9][4] = 0xe7;
	Inv_S_box[10][4] = 0x1d;
	Inv_S_box[11][4] = 0xc6;
	Inv_S_box[12][4] = 0x88;
	Inv_S_box[13][4] = 0x19;
	Inv_S_box[14][4] = 0xae;
	Inv_S_box[15][4] = 0xba;
	Inv_S_box[0][5] = 0x36;
	Inv_S_box[1][5] = 0x2f;
	Inv_S_box[2][5] = 0xc2;
	Inv_S_box[3][5] = 0xd9;
	Inv_S_box[4][5] = 0x68;
	Inv_S_box[5][5] = 0xed;
	Inv_S_box[6][5] = 0xbc;
	Inv_S_box[7][5] = 0x3f;
	Inv_S_box[8][5] = 0x67;
	Inv_S_box[9][5] = 0xad;
	Inv_S_box[10][5] = 0x29;
	Inv_S_box[11][5] = 0xd2;
	Inv_S_box[12][5] = 0x07;
	Inv_S_box[13][5] = 0xb5;
	Inv_S_box[14][5] = 0x2a;
	Inv_S_box[15][5] = 0x77;
	Inv_S_box[0][6] = 0xa5;
	Inv_S_box[1][6] = 0xff;
	Inv_S_box[2][6] = 0x23;
	Inv_S_box[3][6] = 0x24;
	Inv_S_box[4][6] = 0x98;
	Inv_S_box[5][6] = 0xb9;
	Inv_S_box[6][6] = 0xd3;
	Inv_S_box[7][6] = 0x0f;
	Inv_S_box[8][6] = 0xdc;
	Inv_S_box[9][6] = 0x35;
	Inv_S_box[10][6] = 0xc5;
	Inv_S_box[11][6] = 0x79;
	Inv_S_box[12][6] = 0xc7;
	Inv_S_box[13][6] = 0x4a;
	Inv_S_box[14][6] = 0xf5;
	Inv_S_box[15][6] = 0xd6;
	Inv_S_box[0][7] = 0x38;
	Inv_S_box[1][7] = 0x87;
	Inv_S_box[2][7] = 0x3d;
	Inv_S_box[3][7] = 0xb2;
	Inv_S_box[4][7] = 0x16;
	Inv_S_box[5][7] = 0xda;
	Inv_S_box[6][7] = 0x0a;
	Inv_S_box[7][7] = 0x02;
	Inv_S_box[8][7] = 0xea;
	Inv_S_box[9][7] = 0x85;
	Inv_S_box[10][7] = 0x89;
	Inv_S_box[11][7] = 0x20;
	Inv_S_box[12][7] = 0x31;
	Inv_S_box[13][7] = 0x0d;
	Inv_S_box[14][7] = 0xb0;
	Inv_S_box[15][7] = 0x26;
	Inv_S_box[0][8] = 0xbf;
	Inv_S_box[1][8] = 0x34;
	Inv_S_box[2][8] = 0xee;
	Inv_S_box[3][8] = 0x76;
	Inv_S_box[4][8] = 0xd4;
	Inv_S_box[5][8] = 0x5e;
	Inv_S_box[6][8] = 0xf7;
	Inv_S_box[7][8] = 0xc1;
	Inv_S_box[8][8] = 0x97;
	Inv_S_box[9][8] = 0xe2;
	Inv_S_box[10][8] = 0x6f;
	Inv_S_box[11][8] = 0x9a;
	Inv_S_box[12][8] = 0xb1;
	Inv_S_box[13][8] = 0x2d;
	Inv_S_box[14][8] = 0xc8;
	Inv_S_box[15][8] = 0xe1;
	Inv_S_box[0][9] = 0x40;
	Inv_S_box[1][9] = 0x8e;
	Inv_S_box[2][9] = 0x4c;
	Inv_S_box[3][9] = 0x5b;
	Inv_S_box[4][9] = 0xa4;
	Inv_S_box[5][9] = 0x15;
	Inv_S_box[6][9] = 0xe4;
	Inv_S_box[7][9] = 0xaf;
	Inv_S_box[8][9] = 0xf2;
	Inv_S_box[9][9] = 0xf9;
	Inv_S_box[10][9] = 0xb7;
	Inv_S_box[11][9] = 0xdb;
	Inv_S_box[12][9] = 0x12;
	Inv_S_box[13][9] = 0xe5;
	Inv_S_box[14][9] = 0xeb;
	Inv_S_box[15][9] = 0x69;
	Inv_S_box[0][10] = 0xa3;
	Inv_S_box[1][10] = 0x43;
	Inv_S_box[2][10] = 0x95;
	Inv_S_box[3][10] = 0xa2;
	Inv_S_box[4][10] = 0x5c;
	Inv_S_box[5][10] = 0x46;
	Inv_S_box[6][10] = 0x58;
	Inv_S_box[7][10] = 0xbd;
	Inv_S_box[8][10] = 0xcf;
	Inv_S_box[9][10] = 0x37;
	Inv_S_box[10][10] = 0x62;
	Inv_S_box[11][10] = 0xc0;
	Inv_S_box[12][10] = 0x10;
	Inv_S_box[13][10] = 0x7a;
	Inv_S_box[14][10] = 0xbb;
	Inv_S_box[15][10] = 0x14;
	Inv_S_box[0][11] = 0x9e;
	Inv_S_box[1][11] = 0x44;
	Inv_S_box[2][11] = 0x0b;
	Inv_S_box[3][11] = 0x49;
	Inv_S_box[4][11] = 0xcc;
	Inv_S_box[5][11] = 0x57;
	Inv_S_box[6][11] = 0x05;
	Inv_S_box[7][11] = 0x03;
	Inv_S_box[8][11] = 0xce;
	Inv_S_box[9][11] = 0xe8;
	Inv_S_box[10][11] = 0x0e;
	Inv_S_box[11][11] = 0xfe;
	Inv_S_box[12][11] = 0x59;
	Inv_S_box[13][11] = 0x9f;
	Inv_S_box[14][11] = 0x3c;
	Inv_S_box[15][11] = 0x63;
	Inv_S_box[0][12] = 0x81;
	Inv_S_box[1][12] = 0xc4;
	Inv_S_box[2][12] = 0x42;
	Inv_S_box[3][12] = 0x6d;
	Inv_S_box[4][12] = 0x5d;
	Inv_S_box[5][12] = 0xa7;
	Inv_S_box[6][12] = 0xb8;
	Inv_S_box[7][12] = 0x01;
	Inv_S_box[8][12] = 0xf0;
	Inv_S_box[9][12] = 0x1c;
	Inv_S_box[10][12] = 0xaa;
	Inv_S_box[11][12] = 0x78;
	Inv_S_box[12][12] = 0x27;
	Inv_S_box[13][12] = 0x93;
	Inv_S_box[14][12] = 0x83;
	Inv_S_box[15][12] = 0x55;
	Inv_S_box[0][13] = 0xf3;
	Inv_S_box[1][13] = 0xde;
	Inv_S_box[2][13] = 0xfa;
	Inv_S_box[3][13] = 0x8b;
	Inv_S_box[4][13] = 0x65;
	Inv_S_box[5][13] = 0x8d;
	Inv_S_box[6][13] = 0xb3;
	Inv_S_box[7][13] = 0x13;
	Inv_S_box[8][13] = 0xb4;
	Inv_S_box[9][13] = 0x75;
	Inv_S_box[10][13] = 0x18;
	Inv_S_box[11][13] = 0xcd;
	Inv_S_box[12][13] = 0x80;
	Inv_S_box[13][13] = 0xc9;
	Inv_S_box[14][13] = 0x53;
	Inv_S_box[15][13] = 0x21;
	Inv_S_box[0][14] = 0xd7;
	Inv_S_box[1][14] = 0xe9;
	Inv_S_box[2][14] = 0xc3;
	Inv_S_box[3][14] = 0xd1;
	Inv_S_box[4][14] = 0xb6;
	Inv_S_box[5][14] = 0x9d;
	Inv_S_box[6][14] = 0x45;
	Inv_S_box[7][14] = 0x8a;
	Inv_S_box[8][14] = 0xe6;
	Inv_S_box[9][14] = 0xdf;
	Inv_S_box[10][14] = 0xbe;
	Inv_S_box[11][14] = 0x5a;
	Inv_S_box[12][14] = 0xec;
	Inv_S_box[13][14] = 0x9c;
	Inv_S_box[14][14] = 0x99;
	Inv_S_box[15][14] = 0x0c;
	Inv_S_box[0][15] = 0xfb;
	Inv_S_box[1][15] = 0xcb;
	Inv_S_box[2][15] = 0x4e;
	Inv_S_box[3][15] = 0x25;
	Inv_S_box[4][15] = 0x92;
	Inv_S_box[5][15] = 0x84;
	Inv_S_box[6][15] = 0x06;
	Inv_S_box[7][15] = 0x6b;
	Inv_S_box[8][15] = 0x73;
	Inv_S_box[9][15] = 0x6e;
	Inv_S_box[10][15] = 0x1b;
	Inv_S_box[11][15] = 0xf4;
	Inv_S_box[12][15] = 0x5f;
	Inv_S_box[13][15] = 0xef;
	Inv_S_box[14][15] = 0x61;
	Inv_S_box[15][15] = 0x7d;
	
	unsigned int C[11];
	C[0] = 0x00000000;
	C[1] = 0x01000000;
	C[2] = 0x02000000;
	C[3] = 0x04000000;
	C[4] = 0x08000000;
	C[5] = 0x10000000;
	C[6] = 0x20000000;
	C[7] = 0x40000000;
	C[8] = 0x80000000;
	C[9] = 0x1b000000;
	C[10] = 0x36000000;

	unsigned char M_box[4][4];
	M_box[0][0] = 0x02;
	M_box[1][0] = 0x01;
	M_box[2][0] = 0x01;
	M_box[3][0] = 0x03;
	M_box[0][1] = 0x03;
	M_box[1][1] = 0x02;
	M_box[2][1] = 0x01;
	M_box[3][1] = 0x01;
	M_box[0][2] = 0x01;
	M_box[1][2] = 0x03;
	M_box[2][2] = 0x02;
	M_box[3][2] = 0x01;
	M_box[0][3] = 0x01;
	M_box[1][3] = 0x01;
	M_box[2][3] = 0x03;
	M_box[3][3] = 0x02;

	unsigned char Inv_M_box[4][4];
	Inv_M_box[0][0] = 0x0e;
	Inv_M_box[1][0] = 0x09;
	Inv_M_box[2][0] = 0x0d;
	Inv_M_box[3][0] = 0x0b;
	Inv_M_box[0][1] = 0x0b;
	Inv_M_box[1][1] = 0x0e;
	Inv_M_box[2][1] = 0x09;
	Inv_M_box[3][1] = 0x0d;
	Inv_M_box[0][2] = 0x0d;
	Inv_M_box[1][2] = 0x0b;
	Inv_M_box[2][2] = 0x0e;
	Inv_M_box[3][2] = 0x09;
	Inv_M_box[0][3] = 0x09;
	Inv_M_box[1][3] = 0x0d;
	Inv_M_box[2][3] = 0x0b;
	Inv_M_box[3][3] = 0x0e;

	//Input
	string K = "Thats my Kung Fu";
	string M = "Two One Nine Two";

	string KRes[11];

	ExpandK(K, C, S_box, KRes);
	
	string s = AES128(K, M, C, S_box, M_box);

	cout << endl << "-> Giai ma: " << InvAES128(KRes, s, Inv_S_box, Inv_M_box);

	return 0;
}
