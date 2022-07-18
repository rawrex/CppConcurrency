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
//
// This permits the compiler to use memcpy(), or an equivalent, for assignment operations,
// because there’s no user-written code to run.
//
// Note, the compare-exchange operations do bitwise comparison as if using memcmp,
// rather than using any comparison operator that may be defined for UDT.
//
// If the type provides comparison operations that have different semantics,
// or the type has padding bits that do not participate in normal comparisons,
// then a compare-exchange operation may fail, even though the values compare equally.
//
// The reasoning behind these restrictions goes back to one of the key guidelines:
// don’t pass pointers and references to protected data outside the scope of the lock 
// by passing them as arguments to user-supplied functions.
//
// In general, the compiler isn’t going to be able to generate lock-free code for std::atomic<UDT>,
// so it will have to use an internal lock for all the operations.
// If user-supplied copy-assignment or comparison operators were permitted,
// this would require passing a reference to the protected data as an argument to a user-supplied function.
// Thus, violating the guideline.

int main() {

}
