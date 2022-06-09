#include <cmath>
#include "RandomInt.h"

class Busy {
	RandomInt ri;
	unsigned counter;
	bool isPrime(const unsigned & n) const
	{
		unsigned sqroot = sqrt(n), i = 2;
		for(;i!=sqroot && n%i; ++i);
		return i==sqroot;
	}
public:
	Busy() : ri(1, 999999), counter(0) {}	
	unsigned operator()()
	{
		for(unsigned i = 0; i!=10000; ++i)
			if (isPrime(ri()+ri())) ++counter;
		return counter;
	}
};
