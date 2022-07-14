#include <iostream>
#include <atomic>

// Like std::atomic<bool>, it’s neither copy-constructible nor copy-assignable,
// although it can be both constructed and assigned from the suitable pointer values.
// 
// Has the obligatory is_lock_free(), as well as
// load(), store(), exchange(), compare_exchange_weak(), compare_exchange_strong() member functions,
// All with the same semantics as of the std::atomic<bool>,
//
// Has its own specific operations, the pointer arithmetic operations.
// The basic operations are provided by the fetch_add() and fetch_sub() member functions,
// which do atomic addition and subtraction on the stored address.
// Also, there are the += and -= operators, as well as prefix and postfix -- and ++.

// The operator versions work as you would expect.
//
// E.g. if x is std::atomic<Foo*> to the first entry of an array of Foo objects,
// then x+=3 makes it to point to the 4th entry and returns a plain Foo* that points to that 4th entry.
// fetch_add() and fetch_sub() return the original value and then modify the position of the pointer
// (x.fetch_add(3) will update x to point to the 4th value 
// but will return a pointer to the first value in the array).
//
// This operation is also known as exchange-and-add, and it’s an atomic read-modify-write operation,
// like exchange() and compare_exchange_weak()/compare_exchange_strong().
// As with the other operations, the return value is a plain T* value 
// rather than a reference to the std::atomic<T*> object.

class Foo{};
Foo some_array[5];
std::atomic<Foo*> p(some_array);

Foo* x = p.fetch_add(2);				// Add 2 to the p 
										// And return the value that was there before that

assert(x == some_array);				// Passes
assert(p.load() == &some_array[2]);		// Passes

x = (p -= 1);							// Subtract 1 from the p
										// And return the value that is present after that

assert(x == &some_array[1]);			// Passes
assert(p.load() == &some_array[1]); 	// Passes



// Note, the function forms allows for the memory-ordering semantics to be specified:
p.fetch_add(3, std::memory_order_release);

// While specifying the ordering semantics isn’t possible for the operator forms.
// These forms therefore always have memory_order_seq_cst semantics.


int main() {

}
