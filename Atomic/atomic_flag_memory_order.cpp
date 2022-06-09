#include <iostream>
#include <atomic>

// STORE operation orderings
//	 memory_order_relaxed, memory_order_release, memory_order_seq_cst

// LOAD operations orderings
//	 memory_order_relaxed, memory_order_consume, memory_order_seq_cst,
//	 memory_order_acquire

// READ-MODIFY-WRITE operations orderings
//	memory_order_relaxed, memory_order_release, memory_order_seq_cst,
//	memory_order_acquire, memory_order_acq_rel, memory_order_consume

int main() {

	// Objects of the std::atomic_flag type must be initialized with ATOMIC_FLAG_INIT.
	// This initializes the flag to a clear state. 

	std::atomic_flag f = ATOMIC_FLAG_INIT;

	// It’s the only atomic type to require such special treatment for initialization,
	// but it’s also the only type guaranteed to be lock-free.
	// If the std::atomic_flag object has static storage duration,
	// it’s guaranteed to be statically initialized.
	// Means that there are no initialization-order issues;
	// it will always be initialized by the time of the first use.

	// Once the flag is initialized, there are only three things you can do with it:
	//	- destroy it ~atomic_flag(),
	//	- clear(), a store operation, can’t have memory_order_acquire, memory_order_acq_rel
	//	- test_and_set() set it and query the previous value, a read-modify-write operation

	// E.g.
	// explicitly requests that the flag is cleared with release semantics
	f.clear(std::memory_order_release);
	// uses the default memory ordering for setting the flag and retrieving the old value
	bool b = f.test_and_set();

	// std::atomic_flag is so limited that it can’t even be used as a general Boolean flag, 
	// because it doesn’t have a simple nonmodifying query operation

}
