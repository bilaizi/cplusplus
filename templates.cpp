// Class template specialization
#include <iostream>
 
template<class T>
class myBox {
public:
    T a;
    void print() { std::cout << a << std::endl; }
};

template<>
class myBox<bool> {
public:
    bool a;
    void print() { std::cout << (a ? "true" : "false") << std::endl; }
};

auto main() -> int {
    myBox<bool> box = { true };
    box.print(); // true
    myBox<int> b = { 20 };
    b.print(); // 20
}

// Function template specialization
#include <iostream>
 
template<class T>
class myBox {
    public:
    T a;
    void print();
};
template<class T> void myBox<T>::print() {
    std::cout << a << std::endl;
}
 
template<> void myBox<bool>::print() {
    std::cout << (a ? "true" : "false") << std::endl;
} 

auto main() -> int {
    myBox<bool> a = { true };
    a.print(); // true
    myBox<int> b = { 20 };
    b.print(); // 20
}
