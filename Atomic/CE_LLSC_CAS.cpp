// LL/SC
//
// Load-link returns the current value of a memory location,
// while a subsequent store-conditional to the same memory location will store a new value
// only if no updates have occurred to that location since the load-link.
// Together, this implements a lock-free atomic read-modify-write operation.
// 
// Real implementations of LL/SC do not always succeed
// if there are no concurrent updates to the memory location in question.
// Any exceptional events between the two operations,
// such as a context switch, another load-link, or even (on many platforms)
// another load or store operation, will cause the store-conditional to spuriously fail.
// Older implementations will fail if there are any updates broadcast over the memory bus.
// This is called weak LL/SC by researchers.

// Comparison of LL/SC and compare-and-swap
//
// If any updates have occurred, the store-conditional is guaranteed to fail,
// even if the value read by the load-link has since been restored.
// As such, an LL/SC pair is stronger than a read followed by a compare-and-swap (CAS),
// which will not detect updates if the old value has been restored (the ABA problem).

// Nevertheless, LL/SC is equivalent to CAS 
// in the sense that either primitive can be implemented in terms of the other,
// in O(1) and in a wait-free manner.
