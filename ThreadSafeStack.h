#include <exception>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack: std::exception
{
	const char* what() const noexcept
	{
		return "";
	}
};

template<typename T>
class ThreadSafeStack
{
private:
	std::stack<T> data;
	mutable std::mutex m;
public:
	ThreadSafeStack() = default;

	// Note that we do the work in the body
	// to ensure that the mutex is held across the copy
	ThreadSafeStack(const ThreadSafeStack& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}

	ThreadSafeStack& operator=(const ThreadSafeStack&) = delete;

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(new_value));
	}
	
	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if(data.empty()) throw empty_stack();

		const std::shared_ptr<T> res{std::make_shared<T>(data.top())};
		data.pop();
		return res;
	}
	
	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if(data.empty()) throw empty_stack();

		value=data.top();
		data.pop();
	}
	
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};
