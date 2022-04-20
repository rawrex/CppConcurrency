#include <iostream>
#include <thread>

class HeavyObject;
void swap(HeavyObject& lhs, HeavyObject& rhs);

class Cxx11 {
private:
	HeavyObject some_detail;
	mutable std::mutex mutex;
public:
	Cxx11(some_big_object const& sd) : some_detail(sd) {}

	friend void swap(Cxx11& lhs, Cxx11& rhs)
	{
		// We proceed only if the two are different objects
		if(&lhs==&rhs) return;

		// Lock mutexes of the both objects in deadlock-free manner
		std::lock(lhs.m,rhs.m);
		
		// We supply an additional arg so to indicate that mutexes are already locked
		// and all we need is to adopt the ownership of the existing lock
		std::lock_guard<std::mutex> lock_a(lhs.mutex, std::adopt_lock);
		std::lock_guard<std::mutex> lock_b(rhs.mutex, std::adopt_lock);

		// Do the work
		swap(lhs.some_detail,rhs.some_detail);
	}
};

class Cxx17 {
	HeavyObject some_detail;
	mutable std::mutex mutex;
public:
	friend void swap(Cxx17& lhs, Cxx17& rhs)
	{
		if(&lhs==&rhs) return;
	
		// A variadic template, accepting a list of mutex types as template parameters,
		// and a list of mutexes as constructor arguments. 
		//
		// Class type deduction allows to shorten from this:
		// std::scoped_lock<std::mutex,std::mutex> guard(lhs.m,rhs.m);
		std::scoped_lock guard(lhs.mutex, rhs.mutex);

		swap(lhs.some_detail,rhs.some_detail);
	}
};
