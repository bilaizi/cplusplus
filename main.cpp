#include <algorithm> 
#include <array>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

using namespace std;

auto main()->int{
    array<int, 3> values = { 11, 22, 33 };
    auto [x, y, z] = values;
    struct Point { double x, y, z; };
    Point point={1.0, 2.0, 3.0};
    auto [x, y, z] = point;
    int* a = nullptr;
    a= new int; 
    if(!a){
    }
    delete a;
    a = nullptr;
    auto b= make_unique<int>();
    unique_ptr<int> c{new int};
    auto d= make_unique<int[]>(10);
    unique_ptr<int[]> e{new int[10]};
    auto f= make_shared<int>();
    shared_ptr<int[]> g{new int[10]};
    const int versionNumberMajor = 2;
    const int versionNumberMinor = 1;
    const std::string productName = "Super Hyper Net Modulator";
	

    
    cout  << __func__ << " " << "Hello, Wandbox!" << endl;
    return 0;
}
struct bad_date : std::exception {
   char const * what() const noexcept override { 
       return "bad_date"; 
   }
};

struct month {
    constexpr month(int m) noexcept : value{m} {}
    int value;
};
static constexpr month jan{1};
static constexpr month feb{2};
static constexpr month mar{3};
static constexpr month apr{4};
static constexpr month may{5};
static constexpr month jun{6};
static constexpr month jul{7};
static constexpr month aug{8};
static constexpr month sep{9};
static constexpr month oct{10};
static constexpr month nov{11};
static constexpr month dec{12};

struct year {
    constexpr year(int y) noexcept : value{y} {}
    int value;
};

// 

/* 
#include <future>
using namespace std;

auto main()->int{
    promise<void> t1, t2;
    promise<int> p;
    auto sf = p.get_future().share();
    //shared_future<int> sf{p.get_future()};
    auto f1 = [&t1, sf] {
        t1.set_value();
	// Wait until parameter is set.
	int parameter = sf.get();
	// ...
    };
    auto f2 = [&t2, sf] {
	t2.set_value();
	// Wait until parameter is set.
	int parameter = sf.get();
	// ...
    };
    // Run both lambda expressions asynchronously.
    // Remember to capture the future returned by async()!
    auto r1 = async(launch::async, f1);
    auto r2 = async(launch::async, f2);
    // Wait until both threads have started.
    t1.get_future().wait();
    t2.get_future().wait();
    // Both threads are now waiting for the parameter.
    // Set the parameter to wake up both of them.
    p.set_value(42);
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
*/
