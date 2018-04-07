#include <iostream>

// C++11
constexpr int factorial(int n) {
	return n <= 1 ? 1 : (n * factorial(n - 1));
}
// c++14
constexpr int factorial(int n) {
	int result = n;
	while(n > 1)
		result *= --n;
	return result;
}

auto main() -> int{
	static_assert(factorial(4) == 24); // compile-time
	volatile int k = 8;
	std::cout << factorial(k) << '\n'; // runtime
}
