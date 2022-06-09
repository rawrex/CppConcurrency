#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include "joining_thread.h"

template <typename T>
class ProtVector {
	using guard = std::lock_guard<std::mutex>;
	std::vector<T> vec;
	mutable std::mutex mutex;
public:
	void push_back(T elem) 
	{
		guard g(mutex);
		vec.push_back(elem);
	}
	void show() const
	{
		guard g(mutex);
		for (const T & elem : vec)
			std::cout << elem << ' ';
		std::cout << std::endl;
	}
};

void add(ProtVector<int> & vec, int i) { vec.push_back(i); }
void show(ProtVector<int> & vec) { vec.show(); }

int main() {
	ProtVector<int> vec;	
	
	std::thread t1(add, std::ref(vec), 11);
	std::thread t11(show, std::ref(vec));
	std::thread t2(add, std::ref(vec), 22);
	std::thread t22(show, std::ref(vec));
	std::thread t3(add, std::ref(vec), 33);
	std::thread t33(show, std::ref(vec));
	
	t1.join();
	t11.join();
	t2.join();
	t22.join();
	t3.join();
	t33.join();
}
