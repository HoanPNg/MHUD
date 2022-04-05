#include<iostream>
#include <cstdlib> 
#include <ctime>


using namespace std;

unsigned __int64 PowerMod(unsigned __int64 x, unsigned __int64 b, unsigned __int64 n) {
	unsigned __int64 r = 1;
	while (b > 0)
	{
		x = x % n;
		if (b & 1 == 1)
		{
			r = (r * x) % n;
			
		}
		x = (x * x) % n;
		b = b >> 1;
	}
	return r;
}

bool miller_rabin(unsigned __int64 n)
{
	unsigned __int64 n_temp = n - 1; 
	int m,k = 0;
	while(n_temp > 0)
	{
		if(n_temp & 1 == 1)
			{
				m = n_temp;
				break;
			}
		k++;
		n_temp >>= 1;
	}
	int ran = 2 + rand() % (n-3);
	
	if (n >= 50)
	{
		ran = 2 + ran % 49; 
	}


	unsigned __int64 b = PowerMod(ran,m,n);

	if(b == 1|| b == n-1)
		return true;
	
	for(int i = 1; i < k; i++)
	{
		b = PowerMod(b,2,n);
		if (b == n-1)
			return true;
	}

	return false;

}
 
bool checkPrime(unsigned __int64 n, int tries)
{
	for(int i = 0; i < tries ; i++)
	{
		if(!miller_rabin(n))
			return false;
	}

	return true;

}

int main() {
	srand((int)time(0));
	unsigned __int64 n;
	cout << "n = "; cin >>n;
	if(checkPrime(n,5))
		cout << "prime";
	else
		cout << "not prime";
}
