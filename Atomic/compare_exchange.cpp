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

	// Weak
	//
	// For compare_exchange_weak(),
	// the store might not be successful even if the original value was equal to the expected,
	// in which case the value of the variable is unchanged and the return value of operation is false.
	// This is most likely to happen on machines that lack a single compare-and-exchange instruction,
	// in which case, the processor can’t guarantee that the operation will be done atomically.
	// Possibly because the thread performing the operation can be switched out in the middle 
	// of the necessary sequence of instructions and another thread scheduled in its place.
	// This is called spurious failure, since the reason for the failure is a function of timing, not vlaues.


	bool expected = true;
	// extern std::atomic<bool> b; // Set somewhere else
	std::atomic<bool> b(false);
		
	JThread t1([&]()
	{
		// Because compare_exchange_weak() can fail spuriously, it must typically be used in a loop:
		while(!b.compare_exchange_weak(expected,false) && !expected);

		// Why do we need the "!expected" check?
		//
		// Omitting it would yield a very similar semantics.
		// Only in a case where another thread might reset the value to false,
		// the semantics could become slightly different:
		//	E.g. If the first exchange fails because it finds b is already true,
		//	Then, with expected now true, without "&& !expected" it loops and tries another useless exchange
		//	of true and true which may well succeed trivially breaking from the while loop,
		//	but could exhibit meaningfully different behaviour if b had meanwhile changed back to false,
		//	in which case the loop would continue and may ultimately set b true yet again before breaking.
	});


	// On platforms where no single hardware CAS (Compare-And-Swap) instruction exist,
	// both the weak and strong version are implemented using LL/SC (Load-Link/Store-Conditional)
	// Such systems as ARM, PowerPC, etc.
	// So we can ask, is there any difference between the following two loops?

	// use LL/SC (or CAS on x86) and ignore/loop on spurious failures
	while (!compare_exchange_weak(..))		/* empty */;

	// use LL/SC (or CAS on x86) and ignore/loop on spurious failures
	while (!compare_exchange_strong(..))	/* empty */;

	// E.g. We know that real LL/SC well may fail spuriously on context switch (see file: ./CE_LLSC_CAS.cpp).
	// The strong would bring its own "loop" to detect a spurious failure and mask it.
	// Note that this loop is also more complicated than a usual CAS loop
	// since it must distinguish between spurious failure and mask it, and failure due to concurrent access.
	// The weak version does not have such inner mechanism.
	//
	// So, in the above example, the strong version will do the same check twice:
	// Once in the inner mechanims and once in our explicit loop.
	// Thus, the weak verison has more chances on giving a better performance.
	//
	// So in a nutshell, C++ gives you two semantics,
	// a "best effort" one (weak) and a "I will do it for sure,
	// no matter how many bad things might happen inbetween" one (strong).


	// Note, we should not tie our mental model to the implementation on our specific platform;
	// the standard library is designed to work with more architectures than you might be aware of.


	// As a bottom line:
	// Why would we want to use weak in a loop instead of strong?
	//
	// Spurious failures are not that often, it's not a big performance hit.
	// In constrast, strong must always check for spurious failure and mask it. Expensive.
	// Thus, weak is used because it is a lot faster than strong on some platforms.

	// When should you use weak and when strong?
	//
	// When a compare-and-exchange is in a loop anyway, by means of logic (not technical necessity).
	// The weak version will yield better performance on some platforms, so the rule of thumb is:
	//	- If you would have to introduce a loop only because of spurious failure, use strong.
	//	- If you have a loop anyway, then use weak.

}
