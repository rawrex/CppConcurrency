// Atomic variables are primarily used to synchronize shared memory accesses between threads.

// Typically one thread creates data, then stores to an atomic.
// Other threads read from this atomic, and when the expected value is seen,
// the data the other thread was creating is going to be complete and visible in this thread.

// The different memory model modes are used to indicate how strong this data-sharing bond is.
// Knowledgeable programmers can utilize the weaker models to make more efficient software.

// Each atomic class has a load() and a store() operation which is utilized to perform assignments:

atomic_var1.store (atomic_var2.load());		// atomic variables
											// vs
var1 = var2;								// regular variables

// These operations also have a second optional parameter 
// which is used to specify the memory model mode to use for synchronization.

// There are 3 modes:
//	1) Sequentially Consistent
//	2) Relaxed
//	3) Acquire/Release (hybrid of the two above)


// Sequentially Consistent
//
// This is the default mode used when none is specified, and it is the most restrictive.
// It can also be explicitly specified via std::memory_order_seq_cst.
// It provides the same restrictions and limitation to moving loads around 
// that sequential programmers are inherently familiar with, except it applies across threads.

-Thread 1-       -Thread 2-
 y = 1            if (x.load() == 2)
 x.store (2);        assert (y == 1)
