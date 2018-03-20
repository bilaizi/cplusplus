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
// sender.cpp
#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio/ts/internet.hpp>
#include <boost/asio/ts/io_context.hpp>
#include <boost/asio/ts/timer.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;
//using boost::asio::ip::udp;
using namespace std::chrono_literals;

constexpr short multicast_port = 30001;
constexpr int max_message_count = 10;

class sender{
public:
    sender(io_context& io_context, ip::address& multicast_address) 
        : endpoint_{ 
            multicast_address, multicast_port 
        }, socket_{ 
            io_context, endpoint_.protocol() 
        }, timer_{ io_context },message_count_{ 0 }{
            do_send();
        }
private:
    void do_send() {
        std::ostringstream os;
        os << "Message " << message_count_++;
        message_ = os.str();
        socket_.async_send_to(
            buffer(message_), 
            endpoint_, 
            [this](boost::system::error_code ec, std::size_t length) {
                if(!ec && message_count_ < max_message_count)
                    do_timeout();
            }
        );
    }
    void do_timeout(){
        timer_.expires_after(1s);
        timer_.async_wait([this](boost::system::error_code ec) { if (!ec) do_send(); });
    }

private:
    ip::udp::endpoint endpoint_;
    ip::udp::socket socket_;
    steady_timer timer_;
    int message_count_;
    std::string message_;
};

auto main(int argc, char* argv[])->int{
    try {
        if (argc != 2) {
            std::cerr << "Usage: sender <multicast_address>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    sender 239.255.0.1\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    sender ff31::8000:1234\n";
            return 1;
        }
        io_context io_context;
        sender s{ io_context, ip::make_address(argv[1]) };
        // sender s(io_context,  ip::make_address(argv[1]));
        io_context.run();
    }catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}

// daytime_client.cpp
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <boost/asio/ts/buffer.hpp>	
#include <boost/asio/ts/internet.hpp>
#include <boost/asio/ts/io_context.hpp>
#include <boost/asio/use_future.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;
//using boost::asio::ip::udp;

void get_daytime(io_context& io_context, const char* hostname){
    try{
        udp::resolver resolver{ io_context };
        std::future<udp::resolver::results_type> endpoints = resolver.async_resolve(udp::v4(), hostname, "daytime", use_future);
        // The async_resolve operation above returns the endpoints as a future value that is not retrieved ...
        udp::socket socket{ io_context, udp::v4() };
        std::array<char, 1> send_buf  = {{ 0 }};
        // ... until here. This call may block.
        auto send_length = socket.async_send_to(buffer(send_buf), *endpoints.get().begin(), use_future);
        // Do other things here while the send completes.
        send_length.get(); // Blocks until the send is complete. Throws any errors.
        std::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        auto recv_length = socket.async_receive_from(buffer(recv_buf), sender_endpoint, use_future);
        // Do other things here while the receive completes.
        std::cout.write(recv_buf.data(), recv_length.get()); // Blocks until receive is complete.
    }catch (std::system_error& e){
        std::cerr << e.what() << std::endl;
    }
}

auto main(int argc, char* argv[])->int{
    try{
        if (argc != 2){
            std::cerr << "Usage: daytime_client <host>" << std::endl;
            return 1;
        }
        // We run the io_context off in its own thread 
        // so that it operates completely asynchronously with respect to the rest of the program.
        io_context io_context;
        auto work = make_work_guard(io_context);
        std::thread thread{ [&io_context](){ io_context.run(); } };
        get_daytime(io_context, argv[1]);
        io_context.stop();
        thread.join();
    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

//bank_account_1.cpp
#include <iostream>
#include <boost/asio/ts/executor.hpp>
#include <boost/asio/thread_pool.hpp>
using namespace boost::asio;
//using boost::asio::post;
//using boost::asio::thread_pool;

// Traditional active object pattern.
// Member functions do not block.
class bank_account{
    int balance_ {};
    mutable thread_pool pool_{1};
public:
    void deposit(int amount) {
        post(pool_, [=] {  balance_ += amount; });
    }
    void withdraw(int amount) {
        post(pool_, [=] { if (balance_ >= amount) balance_ -= amount; });
    }
    void print_balance() const {
        post(pool_, [=] { std::cout << "balance = " << balance_ << "\n"; });
    }
    ~bank_account() {
      pool_.join();
    }
};

int main(){
    bank_account acct;
    acct.deposit(20);
    acct.withdraw(10);
    acct.print_balance();
}

//bank_account_2.cpp
#include <iostream>
#include <boost/asio/ts/executor.hpp>
#include <boost/asio/thread_pool.hpp>
using namespace boost::asio;
//using boost::asio::post;
//using boost::asio::thread_pool;
//using boost::asio::use_future;

// Traditional active object pattern.
// Member functions block until operation is finished.
class bank_account{
    int balance_{};
    mutable thread_pool pool_{ 1 };
public:
    void deposit(int amount) {
        post(pool_, use_future([=] { balance_ += amount; })).get();
    }
    void withdraw(int amount) {
        post(pool_, use_future([=] { if (balance_ >= amount) balance_ -= amount; })).get();
    }
    int balance() const{
        return post(pool_, use_future([=] { return balance_; })).get();
    }
};
int main(){
    bank_account acct;
    acct.deposit(20);
    acct.withdraw(10);
    std::cout << "balance = " << acct.balance() << "\n";
}

// fork_join.cpp
#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <vector>
#include <boost/asio/ts/executor.hpp>
#include <boost/asio/thread_pool.hpp>

using boost::asio::dispatch;
using boost::asio::execution_context;
using boost::asio::thread_pool;
// A fixed-size thread pool used to implement fork/join semantics. 
// Functions are scheduled using a simple FIFO queue. 
// Implementing work stealing, or using a queue based on atomic operations, are left as tasks for the reader.
class fork_join_pool : public execution_context{
public:
    // The constructor starts a thread pool with the specified number of threads.
    // Note that the thread_count is not a fixed limit on the pool's concurrency.
    // Additional threads may temporarily be added to the pool if they join a fork_executor.
    explicit fork_join_pool(std::size_t thread_count = std::thread::hardware_concurrency() * 2)
        : use_count_{ 1 }, threads_{ thread_count } {
            try{
                // Ask each thread in the pool to dequeue and execute functions 
                // until it is time to shut down, i.e. the use count is zero.
                for (thread_count_ = 0; thread_count_ < thread_count; ++thread_count_){
                    dispatch(
                        threads_, 
                        [&]{
                            std::unique_lock lock{ mutex_ };
                            while (use_count_ > 0)
                                if (!execute_next(lock))
                                    condition_.wait(lock);
                        }
                    );
                }
            }catch (...){
                stop_threads();
                threads_.join();
                throw;
            }
        }
    // The destructor waits for the pool to finish executing functions.
    ~fork_join_pool(){ 
        stop_threads();
        threads_.join();
    }
private:
    friend class fork_executor;
    // The base for all functions that are queued in the pool.
    struct function_base{
        std::shared_ptr<std::size_t> work_count_;
        void (*execute_)(std::shared_ptr<function_base>& p);
    };
    // Execute the next function from the queue, if any. 
    // Returns true if a function was executed, and false if the queue was empty.
    bool execute_next(std::unique_lock<std::mutex>& lock) { 
        if (queue_.empty())
            return false;
        auto p{ queue_.front()};
        queue_.pop(); 
        lock.unlock();
        execute(lock, p); 
        return true;
    }
    // Execute a function and decrement the outstanding work.
    void execute(std::unique_lock<std::mutex>& lock, std::shared_ptr<function_base>& p) {
        std::shared_ptr<std::size_t> work_count{ std::move(p->work_count_) };
        try{
            p->execute_(p);
            lock.lock();
            do_work_finished(work_count);
        }catch (...){
            lock.lock();
            do_work_finished(work_count);
            throw;
        }
    }
    // Increment outstanding work.
    void do_work_started(const std::shared_ptr<std::size_t>& work_count) noexcept {
        if (++(*work_count) == 1)
            ++use_count_;
    }
    // Decrement outstanding work. Notify waiting threads if we run out.
    void do_work_finished(const std::shared_ptr<std::size_t>& work_count) noexcept {
        if (--(*work_count) == 0) {
            --use_count_; 
            condition_.notify_all();
        }
    }
    // Dispatch a function, executing it immediately if the queue is already loaded.
    // Otherwise adds the function to the queue and wakes a thread.
    void do_dispatch(std::shared_ptr<function_base> p, const std::shared_ptr<std::size_t>& work_count) {
        std::unique_lock lock{ mutex_ };
        if (queue_.size() > thread_count_ * 16) { 
            do_work_started(work_count); 
            lock.unlock(); 
            execute(lock, p); 
        }else { 
            queue_.push(p); 
            do_work_started(work_count); 
            condition_.notify_one(); 
        }
    }
    // Add a function to the queue and wake a thread.
    void do_post(std::shared_ptr<function_base> p, const std::shared_ptr<std::size_t>& work_count) {
        std::lock_guard lock{ mutex_ };
        queue_.push(p);
        do_work_started(work_count);
        condition_.notify_one();
    }
    // Ask all threads to shut down.
    void stop_threads() {
        std::lock_guard lock{ mutex_ };
        --use_count_;
        condition_.notify_all();
    }
    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<std::shared_ptr<function_base>> queue_;
    std::size_t use_count_;
    std::size_t thread_count_;
    thread_pool threads_;
};
// A class that satisfies the Executor requirements. 
// Every function or piece of work associated with a fork_executor is part of a single, joinable group.
class fork_executor{
public:
    fork_executor(fork_join_pool& ctx)
        : context_{ ctx }, work_count_{ std::make_shared<std::size_t>(0) }{}
    fork_join_pool& context() const noexcept {
        return context_;
    }
    void on_work_started() const noexcept {
        std::lock_guard lock{ context_.mutex_ };
        context_.do_work_started(work_count_);
    }
    void on_work_finished() const noexcept {
       std::lock_guard lock{ context_.mutex_ };
       context_.do_work_finished(work_count_);
    }
    template <class Func, class Alloc>
    void dispatch(Func&& f, const Alloc& a) const {
        auto p{ 
            std::allocate_shared<function<Func>>(
                typename std::allocator_traits<Alloc>::template rebind_alloc<char>(a),
                std::move(f), 
                work_count_
            )
        };
        context_.do_dispatch(p, work_count_);
    }
    template <class Func, class Alloc>
    void post(Func f, const Alloc& a) const {
        auto p{
            std::allocate_shared<function<Func>>(
                typename std::allocator_traits<Alloc>::template rebind_alloc<char>(a),
                std::move(f), 
                work_count_
            )
        };
        context_.do_post(p, work_count_);
    }
    template <class Func, class Alloc>
    void defer(Func&& f, const Alloc& a) const {
        post(std::forward<Func>(f), a);
    }
    friend bool operator==(const fork_executor& a, const fork_executor& b) noexcept {
        return a.work_count_ == b.work_count_;
    }
    friend bool operator!=(const fork_executor& a, const fork_executor& b) noexcept {
        return a.work_count_ != b.work_count_;
    }
    // Block until all work associated with the executor is complete. 
    // While it is waiting, the thread may be borrowed to execute functions from the queue.
    void join() const {
        std::unique_lock lock{ context_.mutex_ };
        while (*work_count_ > 0)
            if (!context_.execute_next(lock))
                context_.condition_.wait(lock);
    }
private:
    template <class Func>
    struct function : fork_join_pool::function_base {
        explicit function(Func f, const std::shared_ptr<std::size_t>& w)
            : function_(std::move(f)) {
                work_count_ = w;
                execute_ = [](std::shared_ptr<fork_join_pool::function_base>& p) { 
                    Func tmp(std::move(static_cast<function*>(p.get())->function_));
                    p.reset();
                    tmp();
                };
            }
        Func function_;
    };
    fork_join_pool& context_;
    std::shared_ptr<std::size_t> work_count_;
};
// Helper class to automatically join a fork_executor when exiting a scope.
class join_guard{
public:
    explicit join_guard(const fork_executor& ex) : ex_{ ex } {}
    join_guard(const join_guard&) = delete;
    join_guard(join_guard&&) = delete;
    ~join_guard() { ex_.join(); }
private:
    fork_executor ex_;
};
fork_join_pool pool;
template <class Iterator>
void fork_join_sort(Iterator begin, Iterator end){
      std::size_t n = end - begin;
      if (n > 32768){
          {
              fork_executor fork{ pool };
              join_guard join{ fork };
              dispatch(fork, [=]{ fork_join_sort(begin, begin + n / 2); });
              dispatch(fork, [=]{ fork_join_sort(begin + n / 2, end); });
          }
          std::inplace_merge(begin, begin + n / 2, end);
      }else {
          std::sort(begin, end);
      }
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cerr << "Usage: fork_join <size>\n";
        return 1;
    }
    std::vector<double> vec(std::atoll(argv[1]));
    std::iota(vec.begin(), vec.end(), 0);
    std::random_device rd;
    std::shuffle(vec.begin(), vec.end(), g);
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    fork_join_sort(vec.begin(), vec.end());
    std::chrono::steady_clock::duration elapsed = std::chrono::steady_clock::now() - start;
    std::cout << "sort took ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << " microseconds" << std::endl;
}

// actor.cpp
#include <condition_variable>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <typeinfo>
#include <vector>
#include <boost/asio/ts/executor.hpp>
#include <boost/asio/thread_pool.hpp>
using namespace boost::asio;

using boost::asio::defer;
using boost::asio::executor;
using boost::asio::post;
using boost::asio::strand;
using boost::asio::thread_pool;

//------------------------------------------------------------------------------
// A tiny actor framework
// ~~~~~~~~~~~~~~~~~~~~~~
class actor;

// Used to identify the sender and recipient of messages.
using actor_address = actor*;

// Base class for all registered message handlers.
class message_handler_base{
public:
    virtual ~message_handler_base() {}
    // Used to determine which message handlers receive an incoming message.
    virtual const std::type_info& message_id() const = 0;
};

// Base class for a handler for a specific message type.
template <class Message>
class message_handler : public message_handler_base {
public:
    // Handle an incoming message.
    virtual void handle_message(Message msg, actor_address from) = 0;
};

// Concrete message handler for a specific message type.
template <class Actor, class Message>
class mf_message_handler : public message_handler<Message>{
public:
    // Construct a message handler to invoke the specified member function.
    mf_message_handler(void (Actor::* mf)(Message, actor_address), Actor* a) : function_(mf), actor_(a){}
    // Used to determine which message handlers receive an incoming message.
    virtual const std::type_info& message_id() const {
        return typeid(Message);
    }
    // Handle an incoming message.
    virtual void handle_message(Message msg, actor_address from){
        (actor_->*function_)(std::move(msg), from);
    }
    // Determine whether the message handler represents the specified function.
    bool is_function(void (Actor::* mf)(Message, actor_address)) const {
        return mf == function_;
    }
private:
    void (Actor::* function_)(Message, actor_address);
    Actor* actor_;
};

// Base class for all actors.
class actor{
public:
    virtual ~actor(){}
    // Obtain the actor's address for use as a message sender or recipient.
    actor_address address() {
        return this;
    }
    // Send a message from one actor to another.
    template <class Message>
    friend void send(Message msg, actor_address from, actor_address to) {
        // Execute the message handler in the context of the target's executor.
        post(to->executor_, [=] { to->call_handler(std::move(msg), from); });
    }
protected:
    // Construct the actor to use the specified executor for all message handlers.
    actor(executor e) : executor_{ std::move(e) } { }
    // Register a handler for a specific message type. Duplicates are permitted.
    template <class Actor, class Message>
    void register_handler(void (Actor::* mf)(Message, actor_address)) {
        handlers_.push_back(std::make_shared<mf_message_handler<Actor, Message>>(mf, static_cast<Actor*>(this)));
    }
    // Deregister a handler. Removes only the first matching handler.
    template <class Actor, class Message>
    void deregister_handler(void (Actor::* mf)(Message, actor_address)) {
        const std::type_info& id = typeid(message_handler<Message>);
        for(auto iter = handlers_.begin(); iter != handlers_.end(); ++iter) {
            if((*iter)->message_id() == id) {
                auto mh = static_cast<mf_message_handler<Actor, Message>*>(iter->get());
                if(mh->is_function(mf)) {
                    handlers_.erase(iter);
                    return;
                }
            }
       }
    }
    // Send a message from within a message handler.
    template <class Message>
    void tail_send(Message msg, actor_address to) {
        // Execute the message handler in the context of the target's executor.
        actor* from = this;
        defer(to->executor_, [=] { to->call_handler(std::move(msg), from); });
    }
private:
    // Find the matching message handlers, if any, and call them.
    template <class Message>
    void call_handler(Message msg, actor_address from) {
        const std::type_info& message_id = typeid(Message);
        for(auto& h: handlers_) {
            if(h->message_id() == message_id) {
                auto mh = static_cast<message_handler<Message>*>(h.get());
                mh->handle_message(msg, from);
            }
        }
    }
    // All messages associated with a single actor object should be processed non-concurrently. 
    // We use a strand to ensure non-concurrent execution even if the underlying executor may use multiple threads.
    strand<executor> executor_;
    std::vector<std::shared_ptr<message_handler_base>> handlers_;
};

// A concrete actor that allows synchronous message retrieval.
template <class Message>
class receiver : public actor{
public:
    receiver() : actor{ system_executor() } {
        register_handler(&receiver::message_handler);
    }
    // Block until a message has been received.
    Message wait() {
        std::unique_lock lock{ mutex_ };
        condition_.wait(lock, [this]{ return !message_queue_.empty(); });
        Message msg{ std::move(message_queue_.front()) };
        message_queue_.pop_front();
        return msg;
    }
private:
    // Handle a new message by adding it to the queue and waking a waiter.
    void message_handler(Message msg, actor_address from){
        std::lock_guard lock{ mutex_ };
        message_queue_.push_back(std::move(msg));
        condition_.notify_one();
    }
    std::mutex mutex_;
    std::condition_variable condition_;
    std::deque<Message> message_queue_;
};

class member : public actor{
public:
    explicit member(executor e) : actor{ std::move(e) } {
        register_handler(&member::init_handler);
    }
private:
    void init_handler(actor_address next, actor_address from) {
        next_ = next;
        caller_ = from;
        register_handler(&member::token_handler);
        deregister_handler(&member::init_handler);
    }
    void token_handler(int token, actor_address from) {
        int msg{ token };
        actor_address to{ caller_ };
        if(token > 0){
            msg = token - 1;
            to = next_;
        }
        tail_send(msg, to);
    }
    actor_address next_;
    actor_address caller_;
};

int main(){
    const std::size_t num_threads = 16;
    const int num_hops = 50000000;
    const std::size_t num_actors = 503;
    const int token_value = (num_hops + num_actors - 1) / num_actors;
    const std::size_t actors_per_thread = num_actors / num_threads;
    struct single_thread_pool : thread_pool { 
        single_thread_pool() : thread_pool(1) {} 
    };
    single_thread_pool pools[num_threads];
    std::vector<std::shared_ptr<member>> members(num_actors);
    receiver<int> rcvr;
    // Create the member actors.
    for(std::size_t i{}; i < num_actors; ++i)
        members[i] = std::make_shared<member>(pools[(i / actors_per_thread) % num_threads].get_executor());
    // Initialise the actors by passing each one the address of the next actor in the ring.
    for(std::size_t i{ num_actors }, next_i = 0; i > 0; next_i = --i)
        send(members[next_i]->address(), rcvr.address(), members[i - 1]->address());
    // Send exactly one token to each actor, all with the same initial value, rounding up if required.
    for(std::size_t i{}; i < num_actors; ++i)
        send(token_value, rcvr.address(), members[i]->address());
    // Wait for all signal messages, indicating the tokens have all reached zero.
    for(std::size_t i{}; i < num_actors; ++i)
        rcvr.wait();
}

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
