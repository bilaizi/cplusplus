#include <iostream>
using namespace std;

class MyClass {
 public:
  static int si;
  const static double csd;
  const static int csi = 5;
};
int MyClass::si = 1;
const double MyClass::csd = 1.23;

int main(){
    std::cout << MyClass::si << std::endl; 
    std::cout << MyClass::csd << std::endl; 
    std::cout << MyClass::csi  << std::endl; 
}

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

// Variable templates
#include <iostream>

// Variable templates
template<class T> 
constexpr T pi = T(3.1415926535897932384626433L);

int main(){
    int i = pi<int>; // 3
    float f = pi<float>; // 3.14
    std::cout << i << std::endl; 
    std::cout << f << std::endl; 
}
