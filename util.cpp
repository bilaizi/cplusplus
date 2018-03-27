
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
int a[];
for(int& b : a);//for(auto& b : a); a is modified
for(int b : a);//for(auto b : a);a is unmodified
for(const int& b : a);//for(const auto& b : a); a is modified
for(const int b : a);//for(const auto b : a);a is unmodified

#include <iostream>
#include <memory>
using namespace std;

auto main() -> int {
    auto sp = make_shared<int>(1234);
    auto wp = weak_ptr<int>{ sp };
    if(wp.expired())
        cout << "wp is expired" << endl;
    else
        cout << "wp is not expired" << endl;
    cout << "wp pointing counter = " << wp.use_count() << endl;
    if(auto locked = wp.lock())
        cout << "wp is locked. Value = " << *locked << endl;
    else {
        cout << "wp is unlocked" << endl;
        wp.reset();
    }
    cout << endl;
    sp = nullptr;
    if(wp.expired())
        cout << "wp is expired" << endl;
    else
        cout << "wp is not expired" << endl;
    cout << "wp pointing counter = " << wp.use_count() << endl;
    if(auto locked = wp.lock())
        cout << "wp is locked. Value = " << *locked << endl;
    else {
        cout << "wp is unlocked" << endl;
        wp.reset();
    }
    cout << endl;
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

SFINAE is an acronym for “substitution failure is not an error.” 
It is derived from an arcane process used by C++ compilers during overload resolution.  
At its core, the idea is quite simple: if a candidate function template’s specialization
would lead to an ill-formed (compiler-internal) declaration of that specialization,
just silently ignore that template as is if the compiler has never seen it. 
In another words, the compiler will pretend that wasn’t the template it was looking for. 

With the advent of type query features such as decltype and auto, 
the validity of a function type now entails the validity of expressions, 
since whether decltype(expr) is a valid type depends on whether the operand expr is well-formed.  
Hence the term “Expression SFINAE”.  
It is a little bit more involved because now the compiler has to perform overload resolution 
including potentially unbounded evaluation of constant expressions while it is checking whether a type makes sense.

*/
