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
		catch (...)
		{
			// p.set_exception(...) has the argument type std::exception_ptr,
			// so we cannot pass the std::runtime_error object instance directly.
			p.set_exception(std::current_exception());
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
