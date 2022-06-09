#include <iostream>
#include <latch>

class Data {};
Data makeData(unsigned) { return Data(); }
void processData(Data, unsigned) { return; }
void quickProcessData(Data, unsigned) { return; }
void furtherWork() { return; }


// A latch is a synchronization object 
// that becomes ready when its counter is decremented to zero.
// Once it is ready, it stays ready until it is destroyed.

void foo(){
	// Arbitrary number
	unsigned const thread_count = 4;

	// We specify the initial counter value as the only argument to the ctor
	std::experimental::latch done(thread_count);

	Data data[thread_count];
	std::vector<std::future<void>> threads;

	for(unsigned i = 0; i < thread_count; ++i)
		// The lambda captures everything by reference except i, which is captured by value.
		// We wouldn't want the i to be a reference since it will be updated on the next iteration
		threads.push_back(std::async(std::launch::async, [&, i]()
		{
			data[i] = makeData(i);
			// Each thread calls count_down when done generating the data
			// the latch becomes ready when that count reaches zero
			done.count_down();
			// Do more work
			furtherWork();
		}));

	// If you only need to check if it is ready, then you can call is_ready:
	if (done.is_ready())
		quickProcessData(data, thread_count);

	// If you need to both count down the counter and then wait for the counter,
	// you can call count_down_and_wait:
	done.count_down_and_wait();
	
	// If you need to wait for the latch to become ready, 
	// then you can call wait on the latch:
	done.wait();

	// The data processing at will potentially run concurrently 
	// with the final processing steps of each thread, the furtherWork function
	processData(data, thread_count);
}

int main() {

}
