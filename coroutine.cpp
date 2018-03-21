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
/*
#include <future>
#include <iostream>
#include <experimental/coroutine>

template <typename... Args> 
struct std::experimental::coroutine_traits<std::future<void>, Args...> { 
  struct promise_type { 
           std::promise<void> p;
           void unhandled_exception() {}
           auto get_return_object() { return p.get_future(); } 
           std::experimental::suspend_never initial_suspend() { return {}; } 
           std::experimental::suspend_never final_suspend() { return {}; } 
           void set_exception(std::exception_ptr e) { p.set_exception(std::move(e)); } 
           void return_void() { p.set_value(); } 
  }; 
}; 

template <typename R, typename... Args> 
struct std::experimental::coroutine_traits<std::future<R>, Args...> { 
  struct promise_type { 
    std::promise<R> p; 
    void unhandled_exception() {}
    auto get_return_object() { return p.get_future(); } 
    std::experimental::suspend_never initial_suspend() { return {}; } 
    std::experimental::suspend_never final_suspend() { return {}; } 
    void set_exception(std::exception_ptr e) { p.set_exception(std::move(e)); } 
    template <typename U> void return_value(U &&u) { 
      p.set_value(std::forward<U>(u)); 
    } 
  }; 
}; 

std::future<void> f() { 
  puts("Hi!"); 
  co_return; 
} 
  
std::future<int> g() { 
  co_return 42; 
} 

int main() {
    f().get(); 
    printf("%d\n", g().get()); 
    return 0;
}
*/
