#include <ctime>
#include <random>
#include <utility>
#include <iostream>
#include "JThread.h"
#include <functional>
#include "threadsafe_stack.h"

// Type aliases for our operands
using operands = std::pair<unsigned, unsigned>;
using operands_stack = threadsafe_stack<operands>;

// A functor that generates a random unsigned integer in the specified range 
class RandomInt {
	std::uniform_int_distribution<unsigned> dist;
	std::default_random_engine engine;
public:
	RandomInt(std::size_t a, std::size_t b) : dist(a, b), engine(std::time(0)) {}
	const unsigned operator()() 
	{
		return dist(engine);
	}
};

// Make a safe stack with numbers to work with
operands_stack makeStack(RandomInt ri, size_t n)
{
	operands_stack stack;	
	for (unsigned i = 0; i!=n; ++i)
		stack.push({ri(), ri()});
	return stack;	
}

// The function that will do the work in a thread
void work(operands_stack & stack)
{
	while(!stack.empty())
	{
		const auto id = std::this_thread::get_id(); 
		operands a_b;
		stack.pop(a_b);

		const auto 
			a = a_b.first, 
			b = a_b.second, 
			result = a*b;

		std::cout << "ID: " << id << std::endl;
		std::cout << a << " * " << b << " = " << result << std::endl;
	}
}	
	
int main() {
	std::cout << "ID (main): " << std::this_thread::get_id() << std::endl;

	constexpr size_t
		n_threads = 2,
		n_iterations = 10;

	auto stack = makeStack(RandomInt(10000, 99999), n_iterations);
	std::vector<JThread> threads;
	
	// Create and start threads to do the computation
	for (auto i = 0; i != n_threads; ++i)
		threads.emplace_back(work, std::ref(stack));
}
