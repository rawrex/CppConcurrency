#include <iostream>

// A barrier is a reusable synchronization component used for
// internal synchronization between a set of threads.
// With barriers, each thread can only arrive at the barrier once per cycle.

// There are std::experimental::barrier and std::experimental::flex_barrier

// All the threads must have completed their processing
// before the next data item can be processed

// You construct a barrier with a count specifying the number of threads 
// involved in the synchronization group.

// As each thread is done with its processing,
// it arrives at the barrier and waits for the rest of the group 
// by calling arrive_and_wait() on the barrier

// Barriers release the waiting threads and then reset so they can be used again.
// They also only synchronize within a group of threads.
// A thread cannot wait for a barrier to be ready unless it is in the group.
// Threads can explicitly drop out of the group by calling arrive_and_drop on the barrier,
// In which case that thread cannot wait for the barrier to be ready anymore,
// and the count of threads that must arrive in the next cycle is one less than the current

class Result {};
class DataBlock {};
class DataChunk {};
class Source {};
class Sink {};

Result process(DataChunk);
std::vector<DataChunk> divide_into_chunks(DataBlock data, unsigned n_threads);

void process_data(Source& source, Sink& sink) {

	unsigned const concurrency = std::thread::hardware_concurrency();
	unsigned const n_threads = (concurrency > 0) ? concurrency : 2;

	std::experimental::barrier sync(n_threads);
	std::vector<JThread> threads(n_threads);

	std::vector<DataChunk> chunks;
	Result result;

	for (unsigned i = 0; i < num_threads; ++i)
	{
		threads[i] = joining_thread([&, i]
		{
			while (!source.done()) 
			{
				// First serial region,
				// Splitting each block of data into n_threads chunks
				if (!i)
				{
					Data current_block = source.get_next_data_block();
					chunks = divide_into_chunks(current_block, n_threads);
				}

				// All threads then wait on the barrier for that serial code to complete
				sync.arrive_and_wait();

				// Before you reach the parallel region, 
				// where each thread processes its individual chunk 
				// and updates the result with that data before synchronizing again
				result.set_chunk(i, n_threads, process(chunks[i]));

				// Synchronize again
				sync.arrive_and_wait();

				// Second serial region 
				// Where only thread 0 writes the result out to the sink 
				if (!i) sink.write_data(std::move(result));

				// All threads then keep looping until the source is done...
			}
		}
	}
}
					
// In the flex_barrier there is an additional constructor that takes
// a completion function, as well as a thread count. This function is run on exactly one 
// of the threads that arrived at the barrier, once all the threads have arrived at the barrier.


int main() {

}
