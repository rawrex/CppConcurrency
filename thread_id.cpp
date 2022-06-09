#include <iostream>
#include <thread>


int main() {
	std::thread t;
	std::cout << "Not a thread value: " << t.get_id() << std::endl;
	std::cout << "This thread ID: " << std::this_thread::get_id() << std::endl;
	// No need for join()
}
