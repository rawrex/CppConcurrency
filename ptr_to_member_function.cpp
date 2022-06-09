#include <iostream>
#include <thread>

class Bar {
public:
	void bar() { std::cout << "Bar::bar()" << std::endl; }
};

int main() {
	Bar b;
	// A pointer to a member function and the object on which to invoke the function
	std::thread t(&Bar::bar, &b);
	t.join();
}
