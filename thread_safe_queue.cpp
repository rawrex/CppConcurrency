#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>

template <class T, class Container = std::deque<T>>
class TSQueue {
public:
	// Ctors
	explicit TSQueue(const Container&);
	explicit TSQueue(Container&& = Container());
	template <typename Alloc> 
		explicit queue(const Alloc&);
	template <typename Alloc> 
		queue(const Container&, const Alloc&);
	template <typename Alloc> 
		queue(Container&&, const Alloc&);
	template <typename Alloc> 
		queue(queue&&, const Alloc&);

	void swap(queue& q);

	// Sizing
	bool empty() const;
	size_type size() const;

	// Access
	T& front();
	const T& front() const;
	T& back();
	const T& back() const;

	// Managing elements
	void push(const T& x);
	void push(T&& x);
	void pop();
	template <class... Args> void emplace(Args&&... args);	

};

int main() {}
