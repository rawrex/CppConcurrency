#include <iostream>
#include <future>
#include "../Busy.h" 
#include "../JThread.h"


int main() {
	
	std::promise<unsigned> p;
	std::future<unsigned> f = p.get_future();

	std::cout<< "main_id: " << std::this_thread::get_id() <<std::endl;

	JThread t1([](std::promise<unsigned> prom) 
	{ 
		std::cout<< "start; id: " << std::this_thread::get_id() <<std::endl;

		auto result = Busy()();
		prom.set_value(result); 

		std::cout<< "end" <<std::endl;

		// Testing out the double set_value() call
		try
		{
			prom.set_value(43);  // set the second value
		}
		catch (std::future_error &e)
		{
			std::cerr<< "caught: " << e.what() <<std::endl;
		}
	},
	std::move(p));
	
	std::cout<< "result: " << f.get() <<std::endl;

	// Same testing for the double set_value() call
	try
	{
		std::cout<< f.get() <<std::endl;  // get the second value
  	}
	catch (std::future_error &e)
	{
		std::cerr<< "caught: " << e.what() <<std::endl;
	}

}
