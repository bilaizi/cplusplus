#include <chrono>
#include <iostream>
//#include <io_context>
//#include <timer>
#include <boost/asio/ts/io_context.hpp>
#include <boost/asio/ts/timer.hpp>
using namespace std::chrono_literals;
//using boost::asio::io_context;
//using boost::asio::steady_timer;
using namespace boost::asio;

int main(int argc, char* argv[]){
    io_context c;
    steady_timer t{c};
    t.expires_after(5s);
    t.wait();
    return 0;
}
/*
// pipeline.cpp
#include <condition_variable>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <boost/asio/ts/executor.hpp>
#include <boost/asio/thread_pool.hpp>
using namespace boost::asio;

using boost::asio::execution_context;
using boost::asio::executor_binder;
using boost::asio::get_associated_executor;
using boost::asio::post;
using boost::asio::system_executor;
using boost::asio::use_future;
using boost::asio::use_service;
using boost::asio::bind_executor;
using boost::asio::thread_pool;

// An executor that launches a new thread for each function submitted to it.
// This class satisfies the Executor requirements.
class thread_executor{
private:
    // Service to track all threads started through a thread_executor.
    class thread_bag : public execution_context::service {
    public:
        using key_type = thread_bag;
        explicit thread_bag(execution_context& ctx) : execution_context::service{ ctx } { }
        void add_thread(std::thread&& t) {
            std::unique_lock lock{ mutex_ };
            threads_.push_back(std::move(t));
        }
    private:
        virtual void shutdown(){
            for(auto& t : threads_)
                t.join();
        }
        std::mutex mutex_;
        std::vector<std::thread> threads_;
     };
public:
     execution_context& context() const noexcept {
         return system_executor().context();
     }
     void on_work_started() const noexcept {
         // This executor doesn't count work.
     }
     void on_work_finished() const noexcept {
         // This executor doesn't count work.
     }
     template <class Func, class Alloc>
     void dispatch(Func&& f, const Alloc& a) const{
         post(std::forward<Func>(f), a);
     }
     template <class Func, class Alloc>
     void post(Func f, const Alloc&) const {
         thread_bag& bag = use_service<thread_bag>(context());
         bag.add_thread(std::thread(std::move(f)));
     }
     template <class Func, class Alloc>
     void defer(Func&& f, const Alloc& a) const {
         post(std::forward<Func>(f), a);
     }
     friend bool operator==(const thread_executor&, const thread_executor&) noexcept {
         return true;
     }
     friend bool operator!=(const thread_executor&, const thread_executor&) noexcept {
         return false;
     }
};

// Base class for all thread-safe queue implementations.
class queue_impl_base{
    template <class> friend class queue_front;
    template <class> friend class queue_back;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};

// Underlying implementation of a thread-safe queue, shared between the queue_front and queue_back classes.
template <class T>
class queue_impl : public queue_impl_base{
    template <class> friend class queue_front;
    template <class> friend class queue_back;
    std::queue<T> queue_;
};

// The front end of a queue between consecutive pipeline stages.
template <class T>
class queue_front{
public:
    using value_type = T ;
    explicit queue_front(std::shared_ptr<queue_impl<T>> impl) : impl_{ impl }{ }
    void push(T t){
        std::unique_lock lock{ impl_->mutex_ };
        impl_->queue_.push(std::move(t));
        impl_->condition_.notify_one();
    }
    void stop() {
        std::unique_lock lock{ impl_->mutex_ };
        impl_->stop_ = true;
        impl_->condition_.notify_one();
    }
private:
    std::shared_ptr<queue_impl<T>> impl_;
};

// The back end of a queue between consecutive pipeline stages.
template <class T>
class queue_back{
public:
    using value_type = T ;
    explicit queue_back(std::shared_ptr<queue_impl<T>> impl): impl_{ impl }{ }
    bool pop(T& t) {
        std::unique_lock lock{ impl_->mutex_ };
        while (impl_->queue_.empty() && !impl_->stop_)
            impl_->condition_.wait(lock);
        if (!impl_->queue_.empty()) {
            t = impl_->queue_.front();
            impl_->queue_.pop();
            return true;
        }
        return false;
    }
private:
    std::shared_ptr<queue_impl<T>> impl_;
};

// Launch the last stage in a pipeline.
template <class T, class F>
std::future<void> pipeline(queue_back<T> in, F f){
    // Get the function's associated executor, defaulting to thread_executor.
    auto ex = get_associated_executor(f, thread_executor{});
    // Run the function, and as we're the last stage return a future so that the caller can wait for the pipeline to finish.
    return post(ex, use_future([in, f]() mutable { f(in); }));
}

// Launch an intermediate stage in a pipeline.
template <class T, class F, class... Tail>
std::future<void> pipeline(queue_back<T> in, F f, Tail... t){
    // Determine the output queue type.
    using output_value_type = typename executor_binder<F, thread_executor>::second_argument_type::value_type ;
    // Create the output queue and its implementation.
    auto out_impl = std::make_shared<queue_impl<output_value_type>>();
    queue_front<output_value_type> out(out_impl);
    queue_back<output_value_type> next_in(out_impl);
    // Get the function's associated executor, defaulting to thread_executor.
    auto ex = get_associated_executor(f, thread_executor{});
    // Run the function.
    post(ex, [in, out, f]() mutable {  f(in, out); out.stop();  });
    // Launch the rest of the pipeline.
    return pipeline(next_in, std::move(t)...);
}

// Launch the first stage in a pipeline.
template <class F, class... Tail>
std::future<void> pipeline(F f, Tail... t){
    // Determine the output queue type.
    using output_value_type = typename executor_binder<F, thread_executor>::argument_type::value_type;
    // Create the output queue and its implementation.
    auto out_impl = std::make_shared<queue_impl<output_value_type>>();
    queue_front<output_value_type> out(out_impl);
    queue_back<output_value_type> next_in(out_impl);
    // Get the function's associated executor, defaulting to thread_executor.
    auto ex = get_associated_executor(f, thread_executor{});
    // Run the function.
    post(ex, [out, f]() mutable { f(out);  out.stop(); });
    // Launch the rest of the pipeline.
    return pipeline(next_in, std::move(t)...);
}

void reader(queue_front<std::string> out){
    std::string line;
    while (std::getline(std::cin, line))
        out.push(line);
}

void filter(queue_back<std::string> in, queue_front<std::string> out){
    std::string line;
    while (in.pop(line))
        if (line.length() > 5)
            out.push(line);
}

void upper(queue_back<std::string> in, queue_front<std::string> out){
    std::string line;
    while (in.pop(line)) {
        std::string new_line;
        for (char c : line)
            new_line.push_back(std::toupper(c));
        out.push(new_line);
    }
}

void writer(queue_back<std::string> in){
    std::size_t count = 0;
    std::string line;
    while (in.pop(line))
        std::cout << count++ << ": " << line << std::endl;
}

int main(){
    thread_pool pool;
    auto f = pipeline(reader, filter, bind_executor(pool, upper), writer);
    f.wait();
}

// priority_scheduler.cpp
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <boost/asio/ts/executor.hpp>

using namespace boost::asio;
//using boost::asio::executor;

class priority_scheduler : public execution_context{
public:
    // A class that satisfies the Executor requirements.
    class executor_type{
    public:
        executor_type(priority_scheduler& ctx, int pri) noexcept : context_{ ctx }, priority_{ pri }{}
        priority_scheduler& context() const noexcept {
            return context_;
        }
        void on_work_started() const noexcept {
            // This executor doesn't count work. Instead, the scheduler simply runs until explicitly stopped.
        }
        void on_work_finished() const noexcept {
            // This executor doesn't count work. Instead, the scheduler simply runs until explicitly stopped.
        }
        template <class Func, class Alloc>
        void dispatch(Func&& f, const Alloc& a) const {
            post(std::forward<Func>(f), a);
        }
        template <class Func, class Alloc>
        void post(Func f, const Alloc& a) const {
            auto p{std::allocate_shared<item<Func>>( 
                    typename std::allocator_traits<Alloc>::template rebind_alloc<char>(a),
                    priority_, 
                    std::move(f)
            )};
            std::lock_guard lock{ context_.mutex_ };
            context_.queue_.push(p);
            context_.condition_.notify_one();
        }
        template <class Func, class Alloc>
        void defer(Func&& f, const Alloc& a) const {
            post(std::forward<Func>(f), a);
        }
        friend bool operator==(const executor_type& a, const executor_type& b) noexcept {
            return &a.context_ == &b.context_;
        }
        friend bool operator!=(const executor_type& a, const executor_type& b) noexcept {
            return &a.context_ != &b.context_;
        }
    private:
        priority_scheduler& context_;
        int priority_;
    };
    executor_type get_executor(int pri = 0) noexcept {
        return executor_type(*const_cast<priority_scheduler*>(this), pri);
    }
    void run() {
        std::unique_lock lock{ mutex_ };
        for(;;) {
            condition_.wait(lock, [&]{ return stopped_ || !queue_.empty(); });
            if (stopped_)
                return;
            auto p{ queue_.top() };
            queue_.pop();
            lock.unlock();
            p->execute_(p);
            lock.lock();
        }
    }   
    void stop(){
        std::lock_guard lock{ mutex_ };
        stopped_ = true;
        condition_.notify_all();
    }

private:
    struct item_base{
        int priority_;
        void (*execute_)(std::shared_ptr<item_base>&);
    };
    template <class Func>
    struct item : item_base{
        item(int pri, Func f) : function_{ std::move(f) } {
            priority_ = pri;
            execute_ = [](std::shared_ptr<item_base>& p) {
                Func tmp(std::move(static_cast<item*>(p.get())->function_));
                p.reset();
                tmp();
            };
        }
        Func function_;
    };
    struct item_comp {
        bool operator()(const std::shared_ptr<item_base>& a, const std::shared_ptr<item_base>& b) {
            return a->priority_ < b->priority_;
        }
    };
    std::mutex mutex_;
    std::condition_variable condition_;
    std::priority_queue<std::shared_ptr<item_base>, std::vector<std::shared_ptr<item_base>>, item_comp> queue_;
    bool stopped_ = false;
};

int main(int argc, char* argv[]){
    priority_scheduler sched;
    auto low = sched.get_executor(0);
    auto med = sched.get_executor(1);
    auto high = sched.get_executor(2);
    dispatch(low, []{ std::cout << "1\n"; });
    dispatch(low, []{ std::cout << "11\n"; });
    dispatch(med, []{ std::cout << "2\n"; });
    dispatch(med, []{ std::cout << "22\n"; });
    dispatch(high, []{ std::cout << "3\n"; });
    dispatch(high, []{ std::cout << "33\n"; });
    dispatch(high, []{ std::cout << "333\n"; });
    dispatch(sched.get_executor(-1), [&]{ sched.stop(); });
    sched.run();
    return 0;
}

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <boost/asio/ts/buffer.hpp>	
#include <boost/asio/ts/executor.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/asio/ts/io_context.hpp>
#include <boost/asio/ts/net.hpp>
#include <boost/asio/ts/netfwd.hpp>
#include <boost/asio/ts/timer.hpp>
#include <boost/asio/ts/socket.hpp>
using namespace std::chrono_literals;
using namespace boost::asio;
using namespace boost::asio::ip;
//
// reference_counted.cpp
// 


// A reference-counted non-modifiable buffer class.
class shared_const_buffer{
public:
    // Construct from a std::string.
    explicit shared_const_buffer(const std::string& data)
        : data_{ new std::vector<char>{ data.begin(), data.end() } }, buffer_{ buffer(*data_) }{
    }
    // Implement the ConstBufferSequence requirements.
    typedef const_buffer value_type;
    typedef const const_buffer* const_iterator;
    const const_buffer* begin() const { return &buffer_; }
    const const_buffer* end() const { return &buffer_ + 1; }
private:
    std::shared_ptr<std::vector<char>> data_;
    const_buffer buffer_;
};

class session : public std::enable_shared_from_this<session>{
public:
    session(tcp::socket socket) : socket_{ std::move(socket) }{}
    void start(){
        do_write();
    }
private:
    void do_write(){
        auto now{ std::time(0) };
        shared_const_buffer buffer{ std::ctime(&now) };
        auto self{ shared_from_this()};
        boost::asio::async_write(socket_, buffer, [this, self](boost::system::error_code, std::size_t ){});
    }
    // The socket used to communicate with the client.
    tcp::socket socket_;
};

class server{
public:
    server(io_context& io_context, short port): acceptor_(io_context, tcp::endpoint(tcp::v4(), port)){
        do_accept();
    }
private:
    void do_accept(){
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket){
                if(!ec)
                    std::make_shared<session>(std::move(socket))->start();
                do_accept();
            }
        );
    }
    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]){
    try{
        if(argc != 2){
            std::cerr << "Usage: reference_counted <port>\n";
            return 1;
        }
        io_context io_context;
        server s{ io_context, atoi(argv[1]) };
        io_context.run();
    }catch (std::exception& e){
        std::cerr << "Exception: " << e.what() << "\n";
    } 
    return 0;
}

#include <cstdlib>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <boost/asio/ts/buffer.hpp>	
#include <boost/asio/ts/executor.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/asio/ts/io_context.hpp>
#include <boost/asio/ts/net.hpp>
#include <boost/asio/ts/netfwd.hpp>
#include <boost/asio/ts/timer.hpp>
#include <boost/asio/ts/socket.hpp>

using namespace std::chrono_literals;
using namespace boost::asio;
using namespace boost::asio::ip;
//using boost::asio::io_context;
//using boost::asio::steady_timer;
//using boost::asio::ip::tcp;
const int max_length = 1024;
class session: public std::enable_shared_from_this<session>{
public:
    session(tcp::socket socket): socket_(std::move(socket)) {}

    void start(){
        do_read();
    }

private:
    void do_read(){
        auto self{ shared_from_this() };
        socket_.async_read_some(
            buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length){
                if(!ec)
                    do_write(length);
            }
        );
    }

    void do_write(std::size_t length){
       auto self{ shared_from_this() };
       boost::asio::async_write(
           socket_,
           buffer(data_, max_length),
           [this, self](boost::system::error_code ec, std::size_t  length){
               if(!ec)
                   do_read();
           }
       );
    }
    tcp::socket socket_;
    char data_[max_length];
};

class server{
public:
    server(io_context& io_context, short port)
        : acceptor_{ io_context, tcp::endpoint{ tcp::v4(), port } }{
        do_accept();
    }

private:
    void do_accept(){
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket){
                if(!ec)
                    std::make_shared<session>(std::move(socket))->start();
                do_accept();
            }
        );
    }
    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]){
    try{
        if(argc != 2){
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }
        io_context io_context;
        server s{ io_context, std::atoi(argv[1]) };
        io_context.run();
    }catch (std::exception& e){
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
*/
