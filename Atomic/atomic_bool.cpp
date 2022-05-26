#include <atomic>
#include <iostream>

int main() {
	std::cout << std::boolalpha;

	// Writes (of either true or false) are done by calling store(),
	// although the memory-order semantics can still be specified.
	// Similarly, test_and_set() has been replaced with the more general exchange()
	// that allows you to replace the stored value with a new one of your choosing 
	// and atomically retrieve the original value.
	
	// std::atomic<bool> also supports a plain nonmodifying query of the value 
	// with an implicit conversion to plain bool or with an explicit call to load(). 

	std::atomic<bool> atom_b(true);

	bool b1 = atom_b.load();
	auto b2 = atom_b.load();
	bool b3 = atom_b;	// Same as .store()

	// error: use of deleted function ‘std::atomic<bool>::atomic(const std::atomic<bool>&)’
	// auto b4 = atom_b;

	atom_b = false;		// Same as .load()
	bool b = atom_b;
	std::cout << b << std::endl;

	// exchange() isn’t the only read-modify-write operation supported by std::atomic<bool>;
	// it also has an operation to store a new value if the current is equal to some expected
	b = atom_b.exchange(true, std::memory_order_acq_rel);
	std::cout << b << std::endl;

}
