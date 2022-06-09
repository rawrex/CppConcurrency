#ifndef LEVELMUTEX
# define LEVELMUTEX

#include <iostream>
#include <vector>
#include <mutex>
#include "JThread.h"

class LevelMutex {
	
	// Current hierarchy level for this particular thread 
	static thread_local unsigned long thread_level;

	// Underlying STL mutex that actually does the protection
	std::mutex mtx;

	unsigned long const mutex_level;
	std::vector<unsigned long> prev_levels;
	
	void check_level()
	{
		// We can only increase the level
		if(thread_level >= mutex_level)
			throw std::logic_error("mutex hierarchy violated");
	}

	void update_level()
	{
		std::cout << 
			"update_level(); old: " << thread_level << 
			"; new: " << mutex_level << ';' << std::endl;

		prev_levels.push_back(thread_level);
		thread_level = mutex_level;
	}

public:
	explicit LevelMutex(unsigned long value = 1) : mutex_level(value)
	{
		std::cout << "LevelMutex(" << value << ')' << std::endl;
	}

	void lock()	
	{
		std::cout << "lock(); thread_id: " << std::this_thread::get_id() << std::endl;
		check_level();
		
		mtx.lock();
		update_level();
	}

	void unlock()
	{
		std::cout << "unlock(); thread_id: " << std::this_thread::get_id() << std::endl;
		if(thread_level != mutex_level)
			throw std::logic_error("mutex hierarchy violated");
		
		unsigned long prev_level = 0;
		if(!prev_levels.empty()) 
		{
			prev_level = prev_levels.back();
			prev_levels.pop_back();
		}
		thread_level = prev_level;
		std::cout << "setting to previous level: " << prev_level << std::endl;
		mtx.unlock();
	}

	bool try_lock()
	{
		check_level();
		if(!mtx.try_lock()) return false;
		update_level();
		return true;
	}
};

thread_local unsigned long
	LevelMutex::thread_level(0);

#endif
