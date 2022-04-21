#include <condition_variable>
#include <mutex>
#include <queue>

class DataChunk {};
std::mutex mtx;
std::queue<DataChunk> data_queue;
std::condition_variable data_cond;
bool there_is_more_data() { return true; }
bool is_last_chunk(const DataChunk&) { return true; }
const DataChunk prepare_data() { return DataChunk(); }

void data_preparation_thread()
{
	while(there_is_more_data())
	{
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
		std::unique_lock<std::mutex> lk(mtx);

		// The call to wait() on the cond.variable
		// We pass the lock and lambda that expresses the condition we wait on 
		data_cond.wait(lk, []{ return !data_queue.empty(); });

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
		DataChunk data = data_queue.front();
		data_queue.pop();
		
		// We unlock the mutex to do the processing,
		// since we have a copy of the data
		lk.unlock();
		process(data);
		if(is_last_chunk(data)) break;
	}
}
