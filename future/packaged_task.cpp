// If we use a function (pointer) in the std::thread construct,
// the return value of the function will be ignored by std::thread, lost.
// Also if an exception is thrown, std::thread will simply terminate the entire program.
//
// To resolve the interface gap, we have the std::packaged_task class.
// It has a get_future mem.func to return a std::future object to receive a return value.
// It also defines an operator(), to call the original function.
//
// In effect, std::packaged_task<> ties a future to a function or callable object. 

#include <iostream>
#include <exception>
#include <functional>
#include <future>
#include "../JThread.h"

template <typename Function>
class packaged_task;

template <typename Return, typename... Args>
class packaged_task<Return (Args...)> {
	std::promise<Return> promise;
	std::function<Return (Args...)> function;

public:
	packaged_task(std::function<Return (Args...)> func)
		: function(std::move(func)) {}

	void operator()(Args&&... args)
	{
		try
		{
			promise.set_value(function(std::forward<Args&&>(args)...));
		}
		catch (...)
		{
			promise.set_exception(std::current_exception());
		}
	}

	std::future<Return> get_future()
	{
		return promise.get_future();
	}
};

int compute(int a, int b) { return a + b; }

int main() {

	packaged_task<int(int, int)> task(compute);
	std::future<int> future = task.get_future();

	JThread t(std::move(task), 3, 4);

	std::cout << future.get() << std::endl;
}

