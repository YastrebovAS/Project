#include <iostream>
#include <cmath>
//<math.h> для си
using namespase std;
 int main()
	int n,k,E,fact,ln,x;
	int i = 0;
	int fact = 1;
	cout << "Enter x\n";
	cin >> x;
	cout << "Enter E\n";
	cin >> E;
 {
	while(n>E)
	 {
		i++;
		fact*=i;
		ln =log((x+i)/fact)
		for(j=1; j<=i; j++)
			n*=log;
		if(i%2==0)
			n=-n;
		y+=n;
	 }
	cout << "Y sum equals" << y;
	return 0;
 }
/*
//calculate k**N, return 0 in case of overflow
int K_pow_N(unsigned int K, unsigned int N)
{
	unsigned int P = 1;
	for (unsigned int i = 0; i < N; ++i)
	{
		if (P > UINT_MAX / K)
			return 0;
		P *= K;
	}
	return P;
}

int main()
{
	std::cout << "Enter N\n";
	unsigned int N;
	std::cin >> N;
	std::cout << "Enter K\n";
	int K;
	std::cin >> K;

	//define N!
	unsigned int Nfact = factorial(N);
	if (Nfact == 0) //too much number for factorial calculation
	{
		std::cout << N << "! - integer overflow" << "\n";
		return 0;
	}
	else
	    std::cout << N << "! = " << Nfact << "\n";

	//define K**N!
	unsigned int result = K_pow_N(K, Nfact);

	if (result == 0)
		std::cout << K << "**" << Nfact << " integer overflow\n";
	else
		std::cout << K << "**" << Nfact << " = " << result << "\n";

	
	//define max. value of N for unsigned integer type
	
	N = 0;
	while ((Nfact = factorial(N)) > 0)
	{
		std::cout << N << "! = " << Nfact << "\n";
		N += 1;
	}
	std::cout << "N is in the range from 0 to " << N-1 << " for unsigned integer type\n";
	return N-1;
	*/