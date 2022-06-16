#include <iostream>
#include <atomic>
#include "../JThread.h"

int main() {  

	// The compare-exchange operation is the cornerstone of programming with atomic types;
	// it compares the value of the atomic variable with a supplied expected value and
	// stores the supplied desired value if they’re equal.
	// If the values aren’t equal, the expected value is updated with the current value
	// The return type of the compare-exchange functions is a bool,
	// which is true if the store was performed and false otherwise.

	// For compare_exchange_weak(),
	// the store might not be successful even if the original value was equal to the expected,
	// in which case the value of the variable is unchanged and the return value of operation is false.
	// This is most likely to happen on machines that lack a single compare-and-exchange instruction,
	// in which case, the processor can’t guarantee that the operation will be done atomically.
	// Possibly because the thread performing the operation can be switched out in the middle 
	// of the necessary sequence of instructions and another thread scheduled in its place.
	// So called spurious failure, since the reason for the failure is a function of timing, not vlaues.


	bool expected = true;
	std::atomic<bool> b(false);
		
	JThread t1([&]()
	{
		// Because compare_exchange_weak() can fail spuriously, it must typically be used in a loop:
		while(!b.compare_exchange_weak(expected,false) && !expected)
			std::cout << "t2" << std::endl;
	});

	// On the other hand, compare_exchange_strong() is guaranteed to return false 
	// only if the value wasn’t equal to the expected value.
}
