#include <atomic>

// Source: https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync

// Atomic variables are primarily used to synchronize shared memory accesses between threads.
// Typically one thread creates data, then stores to an atomic.
// Other threads read from this atomic, and when the expected value is seen,
// the data the other thread was creating is going to be complete and visible in this thread.

// The different memory model modes are used to indicate how strong this data-sharing bond is.
// Knowledgeable programmers can utilize the weaker models to make more efficient software.

// Each atomic class has a load() and a store() operation which is utilized to perform assignments:

atom_int_a.store(atom_int_b.load());		// atomic variables
											// vs
int_a = int_b;								// regular variables

// These operations also have a second optional parameter 
// which is used to specify the memory model mode to use for synchronization.

// There are 3 modes:
//	1) Sequentially Consistent
//	2) Relaxed
//	3) Acquire/Release


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

// Thread 1
x.store(10, memory_order_relaxed);
y.store(20, memory_order_relaxed);

// Thread 2
if (x.load(memory_order_relaxed) == 10)
{
	assert(y.load(memory_order_relaxed) == 20);		// assert A 
	y.store(10, memory_order_relaxed);
}

// Thread 3
if (y.load(memory_order_relaxed) == 10);
	assert(x.load(memory_order_relaxed) == 10);		// assert B

// Since these three threads are not synced, either of the assertions may fail.
// Without any happens-before edges, no thread can count on a specific ordering from another thread.
// The only ordering imposed is that once a value for a variable from Thread 1 is observed in Thread 2,
// Thread 2 can not see an “earlier” value for that variable from Thread 1. ie, assuming 'x' is initially 0:

// Thread 1
x.store(1, memory_order_relaxed)
x.store(2, memory_order_relaxed)

// Thread 2
y = x.load(memory_order_relaxed)
z = x.load(memory_order_relaxed)

// The assert cannot fail
// Once the store of 2 is seen by Thread 2, it can no longer see the value 1
assert(y <= z);	

// The relaxed mode is most commonly used when we want a variable to be atomic in nature
// rather than using it to synchronize threads for other shared memory data.


// Acquire/Release
//
// The third mode is a hybrid between the other two, but is more similar to the sequentially consistent,
// except it only applies a happens-before relationship to dependent variables.
// This allows for a relaxing of the syncing required between independent reads of independent writes.

std::atomic_int x = 0, y = 0;

// Thread 1
y.store(20, memory_order_release);

// Thread 2
x.store(10, memory_order_release);

// Thread 3
assert(y.load(memory_order_acquire) == 20 && x.load(memory_order_acquire) == 0)

// Thread 4
assert(y.load(memory_order_acquire) == 0 && x.load(memory_order_acquire) == 10)

// Both of these asserts can pass
// Since there is no ordering imposed between the stores in Thread 1 and Thread 2.
// If this example were written using the sequentially consistent model,
// then one of the stores MUST happen-before the other.

// To make matters a bit more complex, the interactions of non-atomic variables are still the same.
// Any store before an atomic operation must be seen in other threads that synchronize.
// For example:

// Thread 1
y = 20;
x.store(10, memory_order_release);

// Thread 2
if(x.load(memory_order_acquire) == 10)
assert(y == 20);

// Since 'y' is not an atomic variable, the "store" to 'y' happens-before the store to 'x',
// so the assert cannot fail in this case.
// The optimizers must still limit the operations performed on shared memory variables
// around atomic operations.


// Consume
//
// std:memory_order_consume is a further subtle refinement in the release/acquire memory model
// that relaxes the requirements slightly
// by removing the happens before ordering on non-dependent shared variables as well.
