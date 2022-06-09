#include <iostream>
#include <atomic>

int main() {

	// The standard atomic types are not copyable or assignable in the conventional sense,
	// in that they have no copy constructors or copy assignment operators.

	// They do, however, support assignment from and implicit conversion to the corresponding
	// built-in types as well as direct load() and store() member functions, exchange(), 
	// compare_exchange_weak(), and compare_exchange_strong().

	std::atomic_char atomic_ch = 'R';
	char ch = atomic_ch; 

	std::cout << ch << std::endl;

}
