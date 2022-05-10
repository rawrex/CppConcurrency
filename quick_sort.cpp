#include <iostream>

template<typename T>
	std::list<T> sequential_quick_sort(std::list<T> input)
{
	if(input.empty())
		return input;

	std::list<T> result;
 
	// You take the first element as the pivot
	// by slicing it off the front of the list using splice()
	result.splice(result.begin(), input, input.begin());

	// We take a reference to avoid copying in comparisons
	const T& pivot = *result.begin();

	// We divide the sequence into those values less than the pivot 
	// and those not less than the pivot 
	auto divide_point = 
		std::partition(input.begin(), input.end(),
			[&](const T& t){ return t < pivot; });

	std::list<T> lower_part;

	// Populate the lower_part list with the values up to the division point
	// These are less than the pivot
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

	// Create a new list with recursively sorted values up to the pivot
	auto new_lower(sequential_quick_sort(std::move(lower_part)));
	// Create a new list with remaining values in the input
	// These are more than the pivot
	auto new_higher(sequential_quick_sort(std::move(input)));

	// Piece the result together in the correct order
	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower);

	return result;
}


// Because this uses a functional style already, 
// it’s now easy to convert this to a parallel version using futures:

template<typename T>
	std::list<T> parallel_quick_sort(std::list<T> input)
{
	if(input.empty())
		return input;

	std::list<T> result;
 
	// You take the first element as the pivot
	// by slicing it off the front of the list using splice()
	result.splice(result.begin(), input, input.begin());

	// We take a reference to avoid copying in comparisons
	const T& pivot = *result.begin();

	// This still does a lot of the work,
	// and that’s still a sequential call, but it’s good enough for now...
	auto divide_point = 
		std::partition(input.begin(), input.end(),
			[&](const T& t){ return t < pivot; });

	std::list<T> lower_part;

	// Populate the lower_part list with the values up to the division point
	// These are less than the pivot
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

	// Lower portion of the list is sorted in a new thread
	std::future<std::list<T>> 
		new_lower(parallel_quick_sort(std::move(lower_part)));

	// The upper portion of the list is sorted with direct recursion as before
	auto new_higher(parallel_quick_sort(std::move(input)));

	// Piece the result together in the correct order
	result.splice(result.end(), new_higher);
	// We need to get the result from the future
	// get() returns an rvalue reference, so it can be moved out
	result.splice(result.begin(), new_lower.get());

	return result;
}

 

int main() {

}
