#include <iostream>
#include <atomic>

// Given a user-defined type UserType,
// std::atomic<UserType> provides the same interface as std::atomic<bool>.
//
// There's a restriction, the type must have a trivial copy-assignment operator.
// This means that the type must not have any virtual functions or virtual base classes 
// and must use the compiler-generated copy-assignment operator.
// Also, every base class and non-static data member of a user-defined type 
// must also have a trivial copy-assignment operator.


int main() {

}
