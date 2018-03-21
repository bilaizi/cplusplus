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

template <typename R, typename... ArgTypes>
struct coroutine_traits;
using promise_type = typename R::promise_type;
template <typename Promise = void>
struct coroutine_handle;


template <typename... Args> 
struct std::experimental::coroutine_traits<std::future<void>, Args...> { 
  struct promise_type { 
           std::promise<void> p;
           void unhandled_exception() {}
           auto get_return_object() { 
               return p.get_future(); 
           } 
           std::experimental::suspend_never initial_suspend() { return {}; } 
           std::experimental::suspend_never final_suspend() { return {}; } 
           void set_exception(std::exception_ptr e) { 
               p.set_exception(std::move(e)); 
           } 
           void return_void() { 
               p.set_value();
           } 
  }; 
}; 

template <typename R, typename... Args> 
struct std::experimental::coroutine_traits<std::future<R>, Args...> { 
  struct promise_type { 
    std::promise<R> p; 
    void unhandled_exception() {}
    auto get_return_object() { 
        return p.get_future(); 
    } 
    std::experimental::suspend_never initial_suspend() { return {}; } 
    std::experimental::suspend_never final_suspend() { return {}; } 
    void set_exception(std::exception_ptr e) { 
        p.set_exception(std::move(e)); 
    } 
    template <typename U> 
    void return_value(U &&u) { 
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

#include <chrono>
#include <future>
#include <iostream>
#include <experimental/coroutine>
#include <boost/asio/ts/io_context.hpp>
#include <boost/asio/ts/timer.hpp>
using namespace std::chrono_literals;
using namespace boost::asio;

template <typename... Args> 
struct std::experimental::coroutine_traits<std::future<void>, Args...> { 
  struct promise_type { 
           std::promise<void> p;
           void unhandled_exception() {}
           auto get_return_object() { 
               return p.get_future(); 
           } 
           std::experimental::suspend_never initial_suspend() { return {}; } 
           std::experimental::suspend_never final_suspend() { return {}; } 
           void set_exception(std::exception_ptr e) { 
               p.set_exception(std::move(e)); 
           } 
           void return_void() { 
               p.set_value();
           } 
  }; 
}; 
template <typename R, typename P> 
auto async_await(system_timer& t, std::chrono::duration<R, P> d) { 
    struct Awaiter { 
        system_timer& t; 
        std::chrono::duration<R, P> d; 
        boost::system::error_code ec; 
        bool await_ready() { return d.count() == 0; } 
        void await_resume() { 
           if (ec) 
               throw boost::system::system_error(ec); 
        } 
        void await_suspend(std::experimental::coroutine_handle<> coro) { 
            t.expires_after(d); 
            t.async_wait([this, coro] (auto ec) mutable { this->ec = ec; coro.resume(); }); 
        } 
   }; 
   return Awaiter{ t, d }; 
} 
std::future<void> sleepy(io_context& io) { 
    system_timer timer{ io }; 
    co_await async_await(timer, 100ms); 
    puts("tick1"); 
    co_await async_await(timer, 100ms); 
    puts("tick2"); 
    co_await async_await(timer, 100ms); 
    puts("tick3"); 
}
int main() {
    io_context io; 
    sleepy(io);
    io.run();  
    return 0;
}

*/
