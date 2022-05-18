#include <future>
#include <string>
#include <chrono>
#include "../Busy.h"
#include "../JThread.h"


using seconds = std::chrono::seconds;

void sleep(const unsigned long long & n) 
{
	std::this_thread::sleep_for(seconds(n));
}
void print(const std::string& msg, int i) 
{ 
	std::cout << msg << ' ' << i << std::endl;
}

// On the receiver side we can split the receiver ‘wait’ and ‘read’ into two steps
// 
// std::future<T> has three member functions:
// 	- void wait()
//			waits until the object is ready to be read.
//	- future_status wait_for(const std::chrono::duration<...> &)
//			wait until the object is ready to be read or until all the wait time has been used.
// 	- future_status wait_until(const std::chrono::time_point<...> &)
//			wait until the object can be read or the deadline is reached.
// 
// The return values for the last two functions can be:
//	- future_status::ready
//			The object is ready to be read.
//	- future_status::timeout
//			Waiting for timeout.
// 	- future_status::deferred
//			The std::future object corresponds to the sending end of an inert evaluation.

int main() {

	std::promise<int> promise;
	std::future<int> future = promise.get_future();

	JThread t([](std::promise<int> p)
	{
		sleep(5);			// Put this thread to sleep for 5 seconds
		p.set_value(21);	// Then set the value of the promise
	},
	std::move(promise));

	for (int i = 0; /* empty */; ++i)
	{
		print("Waiting attempt:", i);
		auto status = future.wait_for(seconds(1));
		// If the status is not a timeout, we are ready to get the value
		if (status != std::future_status::timeout) break;
	}

	std::cout << future.get() << std::endl;

}

