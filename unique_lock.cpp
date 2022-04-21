#include <mutex>

// std::unique_lock instance doesn’t always own the mutex that it’s associated with 
// We can pass std::adopt_lock to have the lock object manage the lock on a mutex 
// or std::defer_lock to indicate that the mutex should remain unlocked on construction.
// The lock can then be acquired later by calling lock() on the std::unique_lock object

class BigObj {};

void swap(BigObj & lhs, BigObj & rhs);

class Foo {
private:
	BigObj data;
	std::mutex mtx;
public:
	Foo(const BigObj & big) : data(big) {}
	
	friend void swap(Foo& lhs, Foo& rhs)
	{
		if (&lhs == &rhs) return;

		// std::defer_lock leaves mutexes unlocked
		std::unique_lock<std::mutex> lock_a(lhs.mtx, std::defer_lock);
		std::unique_lock<std::mutex> lock_b(rhs.mtx, std::defer_lock);

		// Mutexes are locked only in here		
		std::lock(lock_a,lock_b);
		swap(lhs.data, rhs.data);
	}
};


// One possible use is to allow a function to lock a mutex
// and transfer ownership of that lock to the caller, 
void prepare_data() {}
void work() {}

std::mutex mtx;

std::unique_lock<std::mutex> get_lock()
{
	extern std::mutex mtx;
	std::unique_lock<std::mutex> lk(mtx);
	prepare_data();
	// Because lk is an automatic variable declared within the function,
	// it can be returned directly B, without a call to std:move()
	return lk;
}
void process_data()
{
	std::unique_lock<std::mutex> lk(get_lock());
	// We can rely that the data was correctly protected
	work();
}


// std::unique_lock also allows instances to relinquish their locks before they’re destroyed.
// You can do this with the unlock() member function

int main() {}
