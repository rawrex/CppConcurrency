// std::async will call the function we pass in at a certain point in time.
// The caller of std::async can read the return value of the incoming function
// via the returned from the std::async std::future<T> object.
//
// There are two different execution strategies for std::async:
//	- std::launch::async
//		creates an execution thread, 
//		executes the specified work and returns a std::future<T> object.
//	- std::launch::deferred
//		Returns a std::future<T> object directly
//		and defers the specified work to the call point of std::future<T>::get().

#include <future>
#include <thread>
#include <cassert>
#include <iostream>
#include <type_traits>
#include "../Busy.h"


// Demo implementation
template <typename Function, typename... Args>
// The return type is a future of the same type 
// as the result of the function call with the supplied arguments
std::future<typename std::result_of<Function (Args...)>::type> 
	async(std::launch policy, Func&& func, Args&&... args)
{
	// Our simplifying assumption
	assert(policy == std::launch::async && "only async is supported");

	using Result = typename std::result_of<Function (Args...)>::type;
	
	std::packaged_task<Result (Args...)> task(func);
	std::future<Result> future = task.get_future();

	// Launch a new thread since we have the async policy case here
	std::thread t(std::move(task), args...);
	t.detach();		// Avoid the threads destructor

	return future;
}

// std::launch::deferred 
//
// This execution policy does not create a new execution thread.
// It works by maintaining an additional state inside the std::future<T> object. 
// When the user calls std::future<T>::get, the get member function will execute the incoming function.
// If no one calls std::future<T>::get, async with deferred will not execute the incoming function.

int work(int a, int b) { return a + b; }

int main() {

	// E.g. our packaged_taks example could also be rewritten as
	std::future<int> f = std::async(std::launch::async, work, 3, 4);
	// ...
	std::cout << f.get() << std::endl;

}
