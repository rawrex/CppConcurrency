#include <iostream>
#include <thread>

class Thread {
	std::thread t;
public:
	explicit Thread(std::thread th) : t(std::move(th)) 
	{
		if(!t.joinable())
			throw std::logic_error("No thread");
	}
	
	Thread(Thread&& thread) = default;
	Thread& operator=(Thread&& thread)
	{
		t.join();
		t = std::move(thread.t);
		return *this;
	}

	// If the thread is in joinable state, then join it
	~Thread() 
	{
		t.join();
	}
	
	// Prohibit copying
	Thread(const Thread &) = delete;
	Thread& operator=(const Thread&) = delete;
};

void foo() {
	std::cout << "foo()" << std::endl;
}

int main() {
	Thread thread1{std::thread(foo)};
}
