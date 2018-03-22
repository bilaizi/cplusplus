
#include <algorithm> 
#include <array>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
using namespace std;

auto power(double x, int n)->double

auto main()->int{
    for (int i {-3}; i <= 3; ++i)     // Calculate powers of 8 from -3 to +3
        std::cout << std::setw(10) << power(8.0, i);
    std::cout << std::endl;
    vector<double> numbers{ 1, 2, 4, 8, 16, 32, 64, 128, 256 };
    std::cout << *average<double>(begin(numbers), end(numbers)) << std::endl;
    return 0;
}

// Recursive function to calculate x to the power n
auto power(double x, int n)->double{
    if (n == 0)      return 1.0;
    else if (n > 0)  return x * power(x, n - 1);
    else /* n < 0 */ return 1.0 / power(x, -n);
}

// Solution 1: simply use accumulate to sum, and determine the count using std::distance() 
// (the latter is more general than using iterator arithmetic, end - begin, which only works for random-access iterators)
template <typename T, typename IterType>
auto average(IterType begin, IterType end){
	const auto count = std::distance(begin, end);
	const auto sum = std::accumulate(begin, end, T{});
	return count ? std::optional<T>(sum / count) : std::nullopt;
}
/* Solution 2: accumulate a pair<> that counts both the number of elements and the sum
template <typename T, typename IterType>
auto average(IterType begin, IterType end){
    auto accumulated = std::accumulate(
        begin, 
        end, 
        std::make_pair(0u, T{}), [](const auto& accumulated, const auto& element){
            return std::make_pair(accumulated.first + 1, accumulated.second + element); 
        }
    );
    return accumulated.first? std::optional<T>(accumulated.second / accumulated.first) : std::nullopt;
}
*/

/*
auto main()->int{
    return 0;
}
int main(){
    return 0;
}
auto main(int argc,char** argv)->int{
    return 0;
}
int main(int argc,char** argv){
    return 0;
}
Before C++17 
pair<int, double> p{ 2, 4.5 };
auto t = make_tuple(4, 3, 2.5);
lock_guard<std::mutex> lck{ foo.mtx};
After C++17 
pair p{ 2, 4.5 };
tuple t{ 4, 3, 2.5 };
auto lck = lock_guard{ foo.mtx};
*/
