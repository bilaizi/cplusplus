#include <iostream>
#include <experimental/coroutine>

struct generator {
    struct promise_type;
    using handle = std::experimental::coroutine_handle<promise_type>;
    struct promise_type {
        int current_value;
        static auto get_return_object_on_allocation_failure() { 
            return generator{nullptr}; 
        }
        void unhandled_exception() {}
        void return_void() {}
        auto get_return_object() { 
            return generator{handle::from_promise(*this)}; 
        }
        auto initial_suspend() { 
            return std::experimental::suspend_always{}; 
        }
        auto final_suspend() { 
            return std::experimental::suspend_always{}; 
        }
        auto yield_value(int value) {
            current_value = value;
            return std::experimental::suspend_always{};
        }
    };
    bool move_next() { 
        return coro ? (coro.resume(), !coro.done()) : false; 
    }
    int current_value() { 
        return coro.promise().current_value;
    }
    ~generator() { 
        if(coro) coro.destroy(); 
    }
private:
    generator(handle h) : coro(h) {}
    handle coro;
};
generator f() { 
    co_yield 1; 
    co_yield 2; 
}
int main() {
    auto g = f();
    while (g.move_next()) 
        std::cout << g.current_value() << std::endl;
    return 0;
}
