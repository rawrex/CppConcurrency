#include <iostream>
#include <atomic>
#include "../JThread.h"

int main() {
	
	int expected = 21;
	std::atomic<int> atomic_i(33);

	JThread t1([&]() 
	{
		while(!atomic_i.compare_exchange_weak(expected,true));
	});

	JThread t2([&]() 
	{
		while(!atomic_i.compare_exchange_weak(expected,true));
	});

}
