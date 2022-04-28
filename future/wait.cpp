#include <thread>
#include <vector>
#include <future>
#include <numeric>
#include <iostream>
#include "../Busy.h"
#include "../JThread.h"

void print(const std::string& msg, std::thread::id i)
{
	std::hash<std::thread::id> hasher;
	auto short_id = hasher(i) % 10000000;
	std::cout<< msg << ' ' << short_id <<std::endl;
}
void print(const std::string& msg, int i = 0)
{
	std::cout << msg << ' ';
	if (i) std::cout << i;
	std::cout<<std::endl;
}

int main() {
	std::vector<long long unsigned> vector;
	std::promise<void> promise;
	print("Main will need to initialized the data, main_id:", 
		std::this_thread::get_id());

	JThread t([	&vector ]( std::future<void> f )
	{
		print("\tStart thread. Wait for the data to be ready,"
			" thread_id:",
			std::this_thread::get_id());

		// Wait until the future has a value set (void)
        f.wait();

        print("\tWait is over. Start computation...");
        auto sum = std::accumulate(vector.begin(), vector.end(), 0LL);
        print("\tEnd of computation.");
        print("\tSum:", sum);
	},
	promise.get_future());

	// Slow down the main thread
	// the t thread will print "Start..." before the main will print "Initializing..."
	for (long long int i = 0; i < 1000; ++i)
		Busy()();

	print("Initializing the data");
	for (long long int i = 0; i < 1000; ++i)
		vector.push_back(Busy()());

	// The data is ready, notify the waiting thread
	print("Data is ready. Notify thread.");
	promise.set_value();
}

