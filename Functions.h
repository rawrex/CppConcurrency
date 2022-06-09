#include <thread>

unsigned this_id() {
	std::hash<std::thread::id> hasher;
	auto id = std::this_thread::get_id();
	unsigned short_id = hasher(id) % 100000;
	return short_id;
}
