#include <iostream>
#include <atomic>
#include <string>
#include "../JThread.h"


void print(bool b) 
{
	std::cout << std::boolalpha << b << std::endl;
}

class MyType {

};

int main()
{
	print(std::atomic<int>::is_always_lock_free);	
	print(std::atomic<MyType>::is_always_lock_free);	
	print(std::atomic_char::is_always_lock_free);

}
