#include<iostream>
#include <iomanip>

using namespace std;

string addCharToString(string m, unsigned char achar) {
	unsigned char a[1];
	a[0] = achar;
	string add(a, a + 1);

	return m + add;
}

string Int64ToString(unsigned __int64 I) {
	unsigned char c[8];

	for (int i = 0; i < 8; i++) {
		c[i] = (I >> (56 - i*8)) & 0xff;
	}

	string res(c, c + 8);

	return res;
}

string padMessage(string m) {
	unsigned __int64 len = (m.length())*8;
	string last = Int64ToString(len);

	string res = addCharToString(m, 0b10000000);
	
	int nlen = res.length();
	int npad = nlen % 64;

	if (npad <= 56) {
		npad = 56 - npad;
	}
	else {
		npad = 56 + 64 - npad;
	}

	for (int i = 0; i < npad; i++) {
		res = addCharToString(res, 0b00000000);
	}

	res = res + last;

	return res;
}

unsigned int StringToInts(string K, unsigned int pos) {
	unsigned int res = 0;

	unsigned int i1 = ((K[pos * 4]) << 24) & 0xff000000;
	unsigned int i2 = ((K[pos * 4 + 1]) << 16) & 0x00ff0000;
	unsigned int i3 = ((K[pos * 4 + 2]) << 8) & 0x0000ff00;
	unsigned int i4 = ((K[pos * 4 + 3])) & 0x000000ff;

	res = i1 + i2 + i3 + i4;

	return res;
}

unsigned int LittleEdianInt(unsigned int big) {
	unsigned int i1 = ((big) << 24) & 0xff000000;
	unsigned int i2 = ((big) << 8) & 0x00ff0000;
	unsigned int i3 = ((big) >> 8) & 0x0000ff00;
	unsigned int i4 = ((big) >> 24) & 0x000000ff;

	return i1 + i2 + i3 + i4;
}

string Int32ToString4(unsigned int I) {
	unsigned char c[4];

	c[0] = (I >> 24) & 0xff;
	c[1] = (I >> 16) & 0xff;
	c[2] = (I >> 8) & 0xff;
	c[3] = I & 0xff;

	string res(c, c + 4);

	return res;
}

string IntsToString(unsigned int I[]) {
	string C1 = Int32ToString4(I[0]);
	string C2 = Int32ToString4(I[1]);
	string C3 = Int32ToString4(I[2]);
	string C4 = Int32ToString4(I[3]);

	string res = C1 + C2 + C3 + C4;

	return res;
}

void PrintHex(string K, int size) {
	if (K.length() != size) {
		cout << "* Khong dung do dai" << endl;
		return;
	}

	int n = size / 4;

	for (int i = 0; i < n; i++) {
		unsigned int temp = StringToInts(K, i);
		unsigned int c0 = ((temp >> 24) & 255);
		unsigned int c1 = ((temp >> 16) & 255);
		unsigned int c2 = ((temp >> 8) & 255);
		unsigned int c3 = (temp & 255);

		cout << hex << setw(2) << right << c0 << " " << setw(2) << right << c1 << " " << setw(2) << right << c2 << " " << setw(2) << right << c3 << " ";
	}
	cout << endl;
}

unsigned __int64 myPow(int x, int n) {
	unsigned __int64 res = x;

	for (int i = 0; i < n; i++) {
		res *= x;
	}

	return res;
}

string MD5(string m, unsigned int s[], unsigned int K[]) {
	
	unsigned int a0 = 0x67452301;
	unsigned int b0 = 0xefcdab89;
	unsigned int c0 = 0x98badcfe;
	unsigned int d0 = 0x10325476;


	string mpad = padMessage(m);

	int n = mpad.length()/64;

	for (int j = 0; j < n; j++) {
		string mtemp = mpad.substr(j*64, 64);

		unsigned int A = a0;
		unsigned int B = b0;
		unsigned int C = c0;
		unsigned int D = d0;

		unsigned __int64 p = myPow(2, 31);

		for (int i = 0; i < 64; i++) {
			unsigned int f, g;

			if ((0 <= i) && (i <= 15)) {
				f = (B & C) | ((~B) & D);
				g = i;
			}
			else if ((16 <= i) && (i <= 31)) {
				f = (D & B) | ((~D) & C);
				g = (5 * i + 1) % 16;
			}
			else if ((32 <= i) && (i <= 47)) {
				f = B ^ C ^ D;
				g = (3 * i + 5) % 16;
			}
			else if ((48 <= i) && (i <= 63)) {
				f = C ^ (B | (~D));
				g = (7 * i) % 16;
			}

			f = (f + A) % p;
			f = (f + K[i]) % p;

			unsigned int mint = StringToInts(mtemp, g);
			mint = LittleEdianInt(mint);
			f = (f + mint) % p;

			A = D;
			D = C;
			C = B;

			B = (B + ((f << s[i]) | (f >> (32 - s[i])))) % p;
		}

		a0 = (a0 + A) % p;
		b0 = (b0 + B) % p;
		c0 = (c0 + C) % p;
		d0 = (d0 + D) % p;
	}

	unsigned int pre[] = {LittleEdianInt(a0), LittleEdianInt(b0), LittleEdianInt(c0), LittleEdianInt(d0) };

	string res = IntsToString(pre);
	
	return res;
}

int main() {
	unsigned int s[64];

	s[0] = 7;	s[1] = 12;	s[2] = 17;	s[3] = 22;	s[4] = 7;	s[5] = 12;	s[6] = 17;	s[7] = 22;
	s[8] = 7;	s[9] = 12;	s[10] = 17;	s[11] = 22; s[12] = 7;	s[13] = 12;	s[14] = 17;	s[15] = 22;
	s[16] = 5;	s[17] = 9;	s[18] = 14;	s[19] = 20;	s[20] = 5;	s[21] = 9;	s[22] = 14;	s[23] = 20;
	s[24] = 5;	s[25] = 9;	s[26] = 14;	s[27] = 20;	s[28] = 5;	s[29] = 9;	s[30] = 14;	s[31] = 20;
	s[32] = 4;	s[33] = 11;	s[34] = 16;	s[35] = 23;	s[36] = 4;	s[37] = 11;	s[38] = 16;	s[39] = 23;
	s[40] = 4;	s[41] = 11;	s[42] = 16;	s[43] = 23;	s[44] = 4;	s[45] = 11;	s[46] = 16;	s[47] = 23;
	s[48] = 6;	s[49] = 10;	s[50] = 15;	s[51] = 21;	s[52] = 6;	s[53] = 10;	s[54] = 15;	s[55] = 21;
	s[56] = 6;	s[57] = 10;	s[58] = 15;	s[59] = 21;	s[60] = 6;	s[61] = 10;	s[62] = 15;	s[63] = 21;

	unsigned int K[64];

	unsigned __int64 p = myPow(2, 31);

	for (int i = 0; i < 64; i++) {
		K[i] = (unsigned __int64) floor(abs(sin(i + 1)) * myPow(2, 31)) % p;
	}

	string m = "fit.hcmus";

	string afterhash = MD5(m, s, K);
	
	cout << endl << "-> Message: " << m << endl << "-> MD5    : ";
	PrintHex(afterhash, 16);

	return 0;
}