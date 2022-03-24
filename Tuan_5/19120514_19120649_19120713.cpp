#include<iostream>
#include<iomanip>
#include<sstream>

using namespace std;	//Sửa input tại dòng 410

string IntToChar4(unsigned int I) {
	unsigned char c[4];

	c[0] = (I >> 24) & 0xff;
	c[1] = (I >> 16) & 0xff;
	c[2] = (I >> 8) & 0xff;
	c[3] = I & 0xff;

	string res(c, c + 4);

	return res;
}

unsigned int Char16ToInts(string K, long pos) {
	unsigned int res = 0;

	unsigned int i1 = ((K[pos * 4]) << 24) & 0xff000000;
	unsigned int i2 = ((K[pos * 4 + 1]) << 16) & 0x00ff0000;
	unsigned int i3 = ((K[pos * 4 + 2]) << 8) & 0x0000ff00;
	unsigned int i4 = ((K[pos * 4 + 3])) & 0x000000ff;

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
	if (K.length() != 16) {
		cout << endl;
		return;
	}

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
		res += (bS[3 - i] << (8 * i));
	}

	return res;
}

void ExpandK(string K, unsigned int C[], unsigned char S_box[][16], string KRes[]) {
	if (K.length() != 16) {
		cout << " * Vui long nhap khoa co do lon 128 bit." << endl;
		return;
	}

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

	/* In S box
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			cout << setw(2) << right << hex << (int)S_box[i][j];
			if (j == 15)
				cout << endl;
			else
				cout << " ";
		}
	}
	*/

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

	/* In C
	for (int i = 0; i < 11; i++) {
		cout << hex << C[i] << endl;
	}
	*/

	//Input
	string K = "Thats my Kung Fu";

	string KRes[11];
	for (int i = 0; i < 11; i++) {
		KRes[i] = "";
	}

	//Oputut
	ExpandK(K, C, S_box, KRes);

	for (int i = 0; i < 11; i++) {
		cout << dec << "K" << setw(2) << left << i << ": ";
		PrintHex(KRes[i]);
	}

	return 0;
}