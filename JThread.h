#ifndef JTHREAD
#define JTHREAD

#include <iostream>
#include <vector>
#include <thread>

class JThread {
	std::thread thread;
public:
	JThread() noexcept = default;

	// Ctors	
	template<typename Callable, typename ... Args>
	explicit JThread(Callable&& callable, Args&& ... args) 
		: thread(std::forward<Callable>(callable), std::forward<Args>(args)...) {}
		
	explicit JThread(std::thread t) noexcept 
		: thread(std::move(t)) {}
	
	// Dtor
	~JThread() noexcept
	{
		testJoin();
	}

	// Move ctor
	JThread(JThread&& other) noexcept
		: thread(std::move(other.thread)) {}
	
	// Move assignment
	JThread& operator=(JThread&& rhs) noexcept
	{
		testJoin();
		thread = std::move(rhs.thread);
		return *this;
	}

	JThread& operator=(std::thread rhs) noexcept 
	{
		testJoin();
		thread = std::move(rhs);
		return *this;
	}

	void swap(JThread& other) noexcept
	{
		thread.swap(other.thread);
	}

	std::thread::id get_id() const noexcept
	{
		return thread.get_id();
	}

	bool joinable() const noexcept
	{
		return thread.joinable();
	}

	void join()
	{
		thread.join();
	}

	void detach()
	{
		thread.detach();
	}

	std::thread& as_thread() noexcept
	{
		return thread;
	}

	const std::thread& as_thread() const noexcept
	{
		return thread;
	}

	// Auxilary functions
	void testJoin() noexcept 
	{
		if(joinable()) thread.join();
	}
};
#endif
