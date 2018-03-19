#include <iostream>
#include <string>
#include <thread>

class C {
    std::string s;
public:
    void operator() () const {
        std::cout << "function object in new thread is created by: " + s << std::endl;
    }
    C() = default;
    C(const C& another) {
        s = "copy constructor";
        std::cout << s << std::endl;
    }
    C(C&& another) {
        s = "move constructor";
        std::cout << s << std::endl;
    }
};

auto main()->int {
    C c;
    std::thread t{c};
    t.join();
    std::thread t2{C{}};
    t2.join();
    return 0;
}
/*
copy constructor
move constructor
function object in new thread is created by: move constructor
move constructor
move constructor
function object in new thread is created by: move constructor
*/
