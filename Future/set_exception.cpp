#include <future>
#include <iostream>
#include <thread>
#include "../JThread.h"

int main() {
	std::promise<int> promise;
	std::future<int> f = promise.get_future();

	JThread t([](std::promise<int> p) {
		try 
		{
			throw std::runtime_error("In-thread exception");
		} 
		catch (const std::exception& e)
		{
			// p.set_exception(...) has the argument type std::exception_ptr,
			// so we cannot pass the std::runtime_error object instance directly.
			p.set_exception(std::current_exception());

			// If std::current_exception is called during exception handling 
			// (typically, in a catch clause),
			// captures the current exception object and creates an std::exception_ptr
			// that holds either a copy or a reference to that exception object 
			// (depending on the implementation)

			// An alternative approach is to use the std::make_exception_ptr() 
			// p.set_exception(std::make_exception_ptr(std::runtime_error("In-thread exception")));
        }
	},
	std::move(promise));

	try
	{
		std::cout << f.get() << std::endl;
	} 
	catch (std::runtime_error &exp)
	{
		std::cout << "Main thread: caught: " << exp.what() << std::endl;
	}
}
