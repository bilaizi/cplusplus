#include <memory>

template<class T>
class pointer {
public:
    using element_type = typename std::pointer_traits<T>::element_type;
    pointer(T value): value_{ value } { }
    T operator->() const noexcept {
        return value_;
    }
private:
    T value_;
};

template<class T>
class special {
public:
    special(T* value) : value_{ value } { }
    T* get() const noexcept {
        return value_;
    }
private:
    T* value_;
};
namespace std {
    template<class T>
    struct pointer_traits<special<T> > {
        using pointer = special<T>;
        using element_type = T;
        using difference_type = std::ptrdiff_t;
        template<class U>
        struct rebind_to {
            typedef special<U> type;
        };
        template<class U>
        using rebind = typename rebind_to<U>::type;
        template<class U>
        static pointer pointer_to(U& v) noexcept {
            return pointer{ &v };
        }
        static element_type* to_address(const pointer& v) noexcept {
            return v.get();
        }
    };
} 
int main(){
    int i = 0;
    {
        typedef int* type;
        type p = &i;
        assert(std::to_address(p) == &i);
    }
    {
        typedef pointer<int*> type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    {
        typedef pointer<pointer<int*> > type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    {
        typedef void* type;
        type p = &i;
        assert(std::to_address(p) == &i);
    }
    {
        typedef pointer<void*> type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    {
        typedef const int* type;
        type p = &i;
        assert(std::to_address(p) == &i);
    }
    {
        typedef pointer<const int*> type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    {
        typedef special<int> type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    {
        typedef special<void> type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    {
        typedef special<const int> type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    {
        typedef pointer<special<int> > type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    {
        typedef pointer<special<void> > type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    {
        typedef pointer<special<const int> > type;
        type p(&i);
        assert(std::to_address(p) == &i);
    }
    return 0;
}
/*
#include <iostream>
#include <type_traits>
 
int main(){
    std::cout << std::boolalpha
              << std::is_same_v<std::remove_cvref_t<int>, int> << '\n'
              << std::is_same_v<std::remove_cvref_t<int&>, int> << '\n'
              << std::is_same_v<std::remove_cvref_t<int&&>, int> << '\n'
              << std::is_same_v<std::remove_cvref_t<const int&>, int> << '\n'
              << std::is_same_v<std::remove_cvref_t<const int[2]>, int[2]> << '\n'
              << std::is_same_v<std::remove_cvref_t<const int(&)[2]>, int[2]> << '\n'
              << std::is_same_v<std::remove_cvref_t<int(int)>, int(int)> << '\n';
}
true
true
true
true
true
true
true
*/
