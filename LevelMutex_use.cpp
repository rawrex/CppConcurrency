#include <iostream>
#include "LevelMutex.h"
#include "JThread.h"


LevelMutex mtx1(10);
LevelMutex mtx2(10);
LevelMutex mtx3(30);
LevelMutex mtx4(40);
LevelMutex mtx5(50);


void foo()
{
	std::cout << "foo(); thread_id: " << std::this_thread::get_id() << std::endl;

	// First mutex acquisition
	std::lock_guard<LevelMutex> lock_1(mtx1);

	// Second mutex. Will acquire the resource due to another level of the mutex
	std::lock_guard<LevelMutex> lock_2(mtx3);

	// Will fail
	// mtx2 and mtx1 are of the same level
	std::lock_guard<LevelMutex> lock_fail(mtx2);
	
}

void bar()
{
	std::cout << "bar(); thread_id: " << std::this_thread::get_id() << std::endl;
	std::lock_guard<LevelMutex> lock(mtx1);
	{
		std::lock_guard<LevelMutex> lock(mtx3);
	}
	{
		std::lock_guard<LevelMutex> lock(mtx4);
		{
			std::lock_guard<LevelMutex> lock(mtx5);
		}
	}
}


int main() {
	//JThread t1(foo);
	JThread t2(bar);	
}
