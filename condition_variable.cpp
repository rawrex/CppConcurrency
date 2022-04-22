#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include "JThread.h"

class DataChunk {};
std::mutex mtx;
std::queue<DataChunk> data_queue;
std::condition_variable data_cond;
bool there_is_more_data() { return true; }
bool is_last_chunk(const DataChunk&) { return true; }
void process(const DataChunk & dt) {}
const DataChunk prepare_data() { return DataChunk(); }

void data_preparation_thread()
{
	while(there_is_more_data())
	{
		// We prepare the data without owning the mutex
		const DataChunk data = prepare_data();

		// Narrow down the scope in which we lock the mutex
		{	
			std::lock_guard<std::mutex> lk(mtx);
			data_queue.push(data);
		}

		// Notify any waiting thread immediately after we've unlocked
		// We wouldn't want to notify while we still have a lock
		data_cond.notify_one();
	}
}

void data_processing_thread()
{
	while(true)
	{
		// Note the use of unique_lock instead of plain lock_guard
		std::unique_lock<std::mutex> lk(mtx);

		// We pass the lock and lambda that expresses the condition we wait on 
		data_cond.wait(lk, []{ 
			// The condition may be tested intermedaite amount of times
			// Due to spurious wakes
			std::cout << "wait()" << std::endl; 
			return !data_queue.empty(); 
		});

		// If the condition isn’t satisfied (the lambda returned false),
		// wait() unlocks the mutex and puts the thread in a blocked or waiting state. 
		// When the condition variable is notified by a call to notify_one()
		// from the data-preparation thread, the thread wakes (unblocks it),
		// reacquires the lock on the mutex, and checks the condition again, 
		// returning from wait() with the mutex still locked if the cond is true. 
		// This is why you need the std::unique_lock rather than the std::lock_guard.
		// The waiting thread must be able to unlock the mutex while it’s waiting.

		// If we are here, then the condition has been satisfied
		// and there's at least one element in the queue
		const DataChunk data = data_queue.front();
		data_queue.pop();
		
		// We unlock the mutex to do the processing,
		// since we have a copy of the data
		lk.unlock();

		// Do the work and exit if we done, otherwise loop again
		process(data);
		if(is_last_chunk(data)) break;
	}
}


// Fundamentally, std::condition_variable::wait is an optimization over a busy-wait.
// Conforming but less than ideal implementation of the wait method
template<typename Predicate>
void minimal_wait(std::unique_lock<std::mutex>& lk, Predicate pred)
{
	while(!pred()){
		lk.unlock();
		lk.lock();
	}
}

int main () {
	JThread t1(data_processing_thread);	
	JThread t2(data_preparation_thread);	
}
