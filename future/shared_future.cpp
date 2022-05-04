#include <chrono>
#include <future>
#include <iostream>
#include <sstream>
#include "../JThread.h"

void print(const std::string& msg, int i = 0)
{
	std::cout << msg;
	if (i) std::cout << ' ' << i;
	std::cout << std::endl;
}

int main() {
	std::promise<std::unique_ptr<int>> promise;
	std::future<std::unique_ptr<int>> future = promise.get_future();
	std::shared_future<std::unique_ptr<int>> shared_future = future.share();

	// Note the capture by value
	JThread t1([ shared_future ]() {
		print("t1: waiting");

		// No race condition
        int value = *shared_future.get();

        std::ostringstream ss;
        ss << "t1: " << value << "\n";
		print(ss.str());
	});

	// Note the capture by value
	JThread t2([ shared_future ]() {
		print("t1: waiting");

		// No race condition
        int value = *shared_future.get();

        std::ostringstream ss;
        ss << "t1: " << value << "\n";
		print(ss.str());
	});

	std::this_thread::sleep_for(std::chrono::seconds(1));
	promise.set_value(std::make_unique<int>(42));
}
