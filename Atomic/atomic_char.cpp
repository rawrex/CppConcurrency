#include <iostream>
#include <atomic>
#include "../JThread.h"

int main() {
	std::atomic<char> ch = 'x';	

	JThread t1([&ch]
	{
		loop:
			ch = '0';
		goto loop;	
	});
	JThread t2([&ch]
	{
		loop:
			ch = '1';
		goto loop;	
	});
	
	while (true)
		std::cout << ch << ' ';
}
