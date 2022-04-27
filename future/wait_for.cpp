#include <chrono>
#include "../Busy.h"
#include "../JThread.h"


using sleep_for = std::this_thread::sleep_for;

int main() {
	std::promise<int> prom;
	std::future<int> future = prom.get_future();

	JThread t([](std::promise<int> p)
	{
		sleep_for(5s);
		p.set_value(42);
	},
	std::move(prom));

	for (int i = 0; ; ++i) {
		std::cout << "waiting attempt " << i << " ..." << std::endl;
		std::future_status status = f.wait_for(1s);
	if (status != std::future_status::timeout) break;
	}

	std::cout << f.get() << std::endl;
}

