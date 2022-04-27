#include <random>
#include <ctime>

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
