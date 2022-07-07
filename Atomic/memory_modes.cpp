#include <atomic>

// Source: https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync

// Atomic variables are primarily used to synchronize shared memory accesses between threads.
// Typically one thread creates data, then stores to an atomic.
// Other threads read from this atomic, and when the expected value is seen,
// the data the other thread was creating is going to be complete and visible in this thread.

// The different memory model modes are used to indicate how strong this data-sharing bond is.
// Knowledgeable programmers can utilize the weaker models to make more efficient software.

// Each atomic class has a load() and a store() operation which is utilized to perform assignments:

atom_int_a.store(atom_int_b.load());		// atomic variables (also have operator= overload)
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
// The opposite approach to the above.
//
// Any reads/writes to atomics are safe from data races.
// Relaxed provides just this minimal guarantee, for a single variable.
// It doesn't provide any guarantees for other variables (atomic or not).
//
// All threads agree on the order of operations on every particular atomic variable.
// But it's the case only for invididual variables.
// It's as if relaxed operations propagate between threads with slight unpredictable delays.
// So, for other variables (atomic or not),
// there still may be different ways to interleave operations on them.
// Thus, we use relaxed for anything 
// that doesn't try to use an atomic variable to synchronize access to non-atomic data.
//
// E.g. operations on shared_ptrs that increment the reference count internally use relaxed.
//
// std::memory_order_relaxed allows for much less syncing by removing the happens-before restrictions.
// These types of atomic operations can also have various optimizations performed on them,
// (e.g., dead store removal, a.k.a. DCE, dead-code elimination)

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
// These do everything relaxed does, and more (so it's supposedly slower or equivalent).
// It is a hybrid between the other two (sequentially consistent and relaxed),
// but is more similar to the sequentially consistent,
// except it only applies a happens-before relationship to dependent variables.
//

// E.g.
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

// Allows for a relaxing of the syncing required between independent reads of independent writes.
// Only stores (writes) can use release. Only loads (reads) can use acquire.
// Read-modify-write operations can be both (memory_order_acq_rel), but they don't have to.
//
// Those let you synchronize threads:
//	- Let's say Thread 1 reads/writes to some memory M 
//	  (non-atomic or atomic variables, doesn't matter);
//	- Then Thread 1 performs a release store to a variable A
//	  Then it stops touching that memory;
//	- If Thread 2 then performs an acquire load of the same variable A,
//	  this load is said to synchronize with the corresponding store in Thread 1;
//	- Now Thread 2 can safely read/write to that memory M.
//
// You only synchronize with the latest writer, not preceding writers.
// You can chain synchronizations across multiple threads.


// Consume
//
// std:memory_order_consume is a further subtle refinement in the release/acquire memory model
// that relaxes the requirements slightly
// by removing the happens before ordering on non-dependent shared variables as well.
// Though, apparently no major compiler implements it,
// and they silently replace it with a stronger memory_order_acquire.
// Even the standard itself says to avoid it.
//
// http://eel.is/c++draft/atomics.order#note-1


// Summary
// Its actually not as complex as it sounds, so in an attempt to un-glaze your eyes,
// examine this case for each of the different memory models:

// Thread 1					// Thread 2						// Thread 3
y.store(20);				if (x.load() == 10) {			if (y.load() == 10)
x.store(10);					assert(y.load() == 20)			assert(x.load() == 10)
								y.store(10)
							}


// When 2 threads synchronize in sequentially consistent mode,
// all the visible variables must be flushed through the system so that all threads see the same state.
// Both asserts must therefore be true.
//
// Release/Acquire mode only requires the two threads involved to be synchronized.
// This means that synchronized values are not commutative to other threads.
// I.e. (1 sync 2 && 2 sync 3) != (1 sync 3)
// The assert in thread 2 must still be true since thread 1 and 2 synchronize with x.load().
// Thread 3 is not involved in this synchronization, so when thread 2 and 3 synchronize with y.load(),
// thread 3's assert can fail.
// There has been no synchronization between threads 1 and 3, so no value can be assumed for 'x' there.
//
// If the stores are release and loads are consume the results are the same as release/acquire
// except there may be less hardware synchronization required.
// Why not always use consume?
// Among the other reasons, (see above)
// the reason is that this example doesn't have any shared memory being synchronized.
// You may not see the values of any shared memory before the stores in the synchronization
// unless it is a parameter to the store.
// I.e., its only a synchronization on shared memory variables used to calculate the store value.
//
// If everything were relaxed, then both asserts can fail because there is no synchronization at all.


// As to mixing memory models - don't do it, it rarely makes sense and is too hard to grasp.
