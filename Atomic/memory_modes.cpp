// Source: https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync
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

// Thread 1			// Thread 2
y = 1				if (x.load() == 2)
x.store (2);			assert (y == 1);

// Although x and y are unrelated variables, the memory model specifies that the assert cannot fail.
// The store to 'y' happens-before the store to x in Thread 1.
// If the load of 'x' in Thread 2 gets the results of the store that happened in Thread 1,
// it must see ALL operations that happened before the store in Thread 1, even unrelated ones.
// Thus, the optimizer is not free to reorder the two stores in Thread 1,
// since Thread 2 must see the store to Y as well.


// This also applies to loads as well:

				 // Common
				 a = 0;
				 y = 0;
				 b = 1;
// Thread 1					// Thread 2
x = a.load()				while (y.load() != b)
y.store(b)					{}
while (a.load() == x)		a.store(1)

// Thread 2 loops until the value of 'y' changes, and proceeds to change 'a'.
// Thread 1 is waiting for 'a' to change.
// When normally compiling sequential code,
// the 'while (a.load() == x)' in Thread 1 looks like an infinite loop, and might be optimized as such.
// Instead, the load of 'a' and comparison to 'x' must happen on each iteration of the loop
// in order for thread 1 and 2 to proceed as expected.


// Relaxed
//
// The opposite approach is .
// std::memory_order_relaxed allows for much less syncing by removing the happens-before restrictions.
// These types of atomic operations can also have various optimizations performed on them,
// such as dead store removal (a.k.a. DCE, dead-code elimination, is a compiler optimization
// to remove code which does not affect the program results).


