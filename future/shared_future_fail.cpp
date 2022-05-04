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

	JThread t1([ &future ]() {
		print("t1: waiting");
		// Race condition
        int value = *future.get();

        std::ostringstream ss;
        ss << "t1: " << value << "\n";
		print(ss.str());
	});

	JThread t2([ &future ]() {
		print("t1: waiting");
		// Race condition
        int value = *future.get();

        std::ostringstream ss;
        ss << "t1: " << value << "\n";
		print(ss.str());
	});

	std::this_thread::sleep_for(std::chrono::seconds(1));
	promise.set_value(std::make_unique<int>(42));
}
