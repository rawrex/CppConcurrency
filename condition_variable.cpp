#include <mutex>
#include <condition_variable>
#include <queue>

class DataChunk {};
std::mutex mtx;
std::queue<DataChunk> data_queue;
std::condition_variable data_cond;

void data_preparation_thread()
{
	while(more_data_to_prepare())
	{
		data_chunk const data=prepare_data();
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

		// 
		data_cond.wait(lk, []{ return !data_queue.empty(); });

		DataChunk data = data_queue.front();
		data_queue.pop();
		
		// 
		lk.unlock();
		process(data);
		if(is_last_chunk(data))
			break;
	}
}
