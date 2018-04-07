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

enum class side { BID, ASK };
class order_book {
	template<side S>
	class book_side {
		using compare = std::conditional_t<S == side::BID, std::greater<>, std::less<>>;
		std::vector<price> levels_;
	public:
		void insert(order o) {
			const auto it = lower_bound(begin(levels_), end(levels_), o.price, compare{});
			if(it != end(levels_) && *it != o.price)
				levels_.insert(it, o.price);
		}
		bool match(price p) const {
			return compare{}(levels_.back(), p);
		};
	}; 
	book_side<side::BID> bids_;
	book_side<side::ASK> asks_;
};
