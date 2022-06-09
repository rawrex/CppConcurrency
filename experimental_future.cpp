#include <future>
#include <cassert>
#include <iostream>
#include "experimental_from_git/future"

// “When the data is ready, then do this processing”
// the member function to add a continuation to a future is called then()
// (e.g.) future.then(continuation).

// As always, std::experimental::future only allows the stored value to be retrieved once.
// If that value is being consumed by a continuation, it cannot be accessed by other code.

std::experimental::future<int> find_the_answer() 
{
	return std::experimental::future<int>();
}


// Our continuation function
// Unlike direct calls to std::async or std::thread,
// you cannot pass arguments to a continuation function,
// because the argument is already defined by the library it is passed a ready future 
// that holds the result that triggered the continuation.
// Assuming your find_the_answer function returns an int,
// this function must take a std::experimental::future<int> as its sole parameter:

std::experimental::future<int> find_the_question(std::experimental::future<int> fut_int);
{
	return std::experimental::future<int>();
}

int main() {

	// If an exception were to occur, it will be "passed on" to the continuation,
	// possibly rethrown there
	auto future_1 = find_the_answer();

	// The find_the_question continuation function is scheduled to run 
	// “on an unspecified thread” when the original future is ready.
	auto future_2 = future_1.then(find_the_question);

	assert(!future_1.valid());
	assert(future_2.valid());

}
