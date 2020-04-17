#include <iostream>
#include <memory>

struct Resource {
    bool isFree{true};
};

class Pool {
public:
    //Custom Resource deleter
    struct Deleter {
        //Called by unique_ptr to destroy/free the Resource
        void operator()(Resource* r) {
            if(r)
                r->isFree = true; // Mark the Resource as free
        }
    };
    //auto return type requires C++14
    auto get() {
        //Create the unique_ptr with nullptr   
        auto rp = std::unique_ptr<Resource, Deleter>(nullptr, Deleter());
        //Find the first free Resource 
        for(auto& r : resources) { 
            if(r.isFree) {
                //Found a free Resource 
                r.isFree = false; //Mark the Resource as not-free
                rp.reset(&r); //Reset the unique_ptr to this Resource*
                break;
            }
        }  
        return rp;
    }
private:
    Resource resources[5]; //Cache of Resources
};

using ResourcePtr = std::unique_ptr<Resource, Pool::Deleter>;

int main() {
    Pool pool;
    {
        ResourcePtr rp;
        rp = pool.get(); //OK. Invokes move-assignment 
        if(rp) {
            //Use resource...     
        }
        //Resource is freed.
    }
    //With default function object (std::default_delete). 
    std::cout << sizeof(std::unique_ptr<int>) << "\n"; //8

    //With a custom empty function object. 
    struct CD { void operator()(int* p) { delete p; } };
    std::cout << sizeof(std::unique_ptr<int, CD>) << "\n"; //8

    //With a capture-less lambda. 
    auto l = [](int* p) { delete p; };
    std::cout << sizeof(std::unique_ptr<int, decltype(l)>) << "\n"; //8 

    //With a function pointer. 
    std::cout << sizeof(std::unique_ptr<int, void(*)(int*)>) << "\n"; //16

    //With a std::function. Much more expensive.
    std::cout << sizeof(std::unique_ptr<int, std::function<void(int*)>>) << "\n"; //40
    return 0;
 }
