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
    C(const C& other) {
        s = "copy constructor";
        std::cout << s << std::endl;
    }
    C(C&& other) {
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

/*

// Chapter8
// listing_8.13.cpp
#include <atomic>
#include <thread>
#include <vector>

struct join_threads{
    join_threads(std::vector<std::thread>&){}
};
    
struct barrier{
    std::atomic<unsigned> count;
    std::atomic<unsigned> spaces;
    std::atomic<unsigned> generation;
    barrier(unsigned count_):count{ count_ }, spaces{ count_ }, generation(0){}
    void wait(){
        unsigned const gen=generation.load();
        if(!--spaces){
            spaces=count.load();
            ++generation;
        }else{
            while(generation.load()==gen)
                std::this_thread::yield();
        }
    }

    void done_waiting(){
        --count;
        if(!--spaces){
            spaces=count.load();
            ++generation;
        }
    }
};

template<typename Iterator>
void parallel_partial_sum(Iterator first,Iterator last){
    typedef typename Iterator::value_type value_type;

    struct process_element{
        void operator()(Iterator first, Iterator last, std::vector<value_type>& buffer, unsigned i, barrier& b){
            value_type& ith_element=*(first+i);
            bool update_source=false;
            for(unsigned step{},stride{1};stride<=i;++step,stride*=2){
                value_type const& source=(step%2)? buffer[i] : ith_element;
                value_type& dest=(step%2) ? ith_element : buffer[i];
                value_type const& addend=(step%2) ? buffer[i-stride] : *(first+i-stride);
                dest=source+addend;
                update_source=!(step%2);
                b.wait();
            }
            if(update_source)
                ith_element=buffer[i];
            b.done_waiting();
        }
    };
    unsigned long const length=std::distance(first,last);
    if(length<=1)
        return;
    std::vector<value_type> buffer(length);
    barrier b(length);
    std::vector<std::thread> threads(length-1);
    join_threads joiner(threads);
    Iterator block_start=first;
    for(unsigned long i=0;i<(length-1);++i)
        threads[i]=std::thread(process_element(),first,last,std::ref(buffer),i,std::ref(b));
    process_element()(first,last,buffer,length-1,b);
}

// Chapter9
class thread_pool{
    std::atomic_bool done;
    thread_safe_queue<std::function<void()> > work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;
    void worker_thread(){
        while(!done){
            std::function<void()> task;
            if(work_queue.try_pop(task))
                task();
            else
                std::this_thread::yield();
        }
    }
public:
    thread_pool():done(false),joiner(threads){
        unsigned const thread_count=std::thread::hardware_concurrency();
        try{
            for(unsigned i{};i<thread_count;++i)
                //threads.push_back(std::thread(&thread_pool::worker_thread,this));
		threads.emplace_back(&thread_pool::worker_thread,this);
        }catch(...){
            done=true;
            throw;
        }
    }

    ~thread_pool(){
        done=true;
    }

    template<typename FunctionType>
    void submit(FunctionType f){
        work_queue.push(std::function<void()>(f));
    }
};

#include <deque>
#include <future>
#include <memory>
#include <functional>
#include <iostream>
#include <iostream>

class function_wrapper{
    struct impl_base {
        virtual void call()=0;
        virtual ~impl_base() {}
    };
    std::unique_ptr<impl_base> impl;
    template<typename F>
    struct impl_type: impl_base{
        F f;
        impl_type(F&& f_): f{ std::move(f_) }{}
        void call() { f(); }
    };
public:
    template<typename F>
    function_wrapper(F&& f):impl{ new impl_type<F>{std::move(f)} }{}
    void call() { impl->call(); }
    function_wrapper(function_wrapper&& other):impl{ std::move(other.impl }{}
    function_wrapper& operator=(function_wrapper&& other){
        impl=std::move(other.impl);
        return *this;
    }
    function_wrapper(function_wrapper&)=delete;
    function_wrapper(const function_wrapper&)=delete;
    function_wrapper& operator=(const function_wrapper&)=delete;
};

class thread_pool{
public:
    std::deque<function_wrapper> work_queue;
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f){
        typedef typename std::result_of<FunctionType()>::type result_type;
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.push_back(std::move(task));
        return res;
    }
    // rest as before
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init){
    const unsigned long length=std::distance(first,last);
    if(!length)
        return init;
    unsigned long const block_size=25;
    unsigned long const num_blocks=(length+block_size-1)/block_size;
    std::vector<std::future<T> > futures(num_blocks-1);
    thread_pool pool;
    Iterator block_start=first;
    for(unsigned long i{};i<(num_threads-1);++i){
        Iterator block_end=block_start;
        std::advance(block_end,block_size);
        futures[i]=pool.submit(accumulate_block<Iterator,T>());
        block_start=block_end;
    }
    T last_result=accumulate_block()(block_start,last);
    T result=init;
    for(unsigned long i{};i<(num_blocks-1);++i)
        result+=futures[i].get();
    result += last_result;
    return result;
}

// Chapter10
class X{
    mutable std::mutex m;
    int data;
public:
    X():data{0}{}
    auto get_value() const ->int{
        std::lock_guard guard(m);
        return data;
    }
    void increment(){
        std::lock_guard guard(m);
        ++data;
    }
};
void increment_all(std::vector<X>& v){
    std::for_each(std::execution::par,v.begin(), v.end(), [](X& x){ x.increment(); });
}

#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

struct log_info {
    std::string page;
    time_t visit_time;
    std::string browser;
    // any other fields
};

extern log_info parse_log_line(std::string const &line);
using visit_map_type= std::unordered_map<std::string, unsigned long long>;
visit_map_type
count_visits_per_page(std::vector<std::string> const &log_lines) {
    struct combine_visits {
        visit_map_type
        operator()(visit_map_type lhs, visit_map_type rhs) const {
            if(lhs.size() < rhs.size())
                std::swap(lhs, rhs);
            for(auto const &entry : rhs) {
                lhs[entry.first]+= entry.second;
            }
            return lhs;
        }

        visit_map_type operator()(log_info log, visit_map_type map) const {
            ++map[log.page];
            return map;
        }
        visit_map_type operator()(visit_map_type map, log_info log) const {
            ++map[log.page];
            return map;
        }
        visit_map_type operator()(log_info log1, log_info log2) const {
            visit_map_type map;
            ++map[log1.page];
            ++map[log2.page];
            return map;
        }
    };

    return std::transform_reduce(
        std::execution::par, log_lines.begin(), log_lines.end(),
        visit_map_type(), combine_visits(), parse_log_line);
}

class Y{
    int data;
public:
    Y():data(0){}
    int get_value() const{
        return data;
    }
    void increment(){
        ++data;
    }
};
class ProtectedY{
    std::mutex m;
    std::vector<Y> v;
public:
	void lock(){
         m.lock();
     }
	void unlock(){
         m.unlock();
     }
     std::vector<Y>& get_vec(){
         return v;
     }
};
void increment_all(ProtectedY& data){
    std::lock_guard guard(data);
    auto& v=data.get_vec();
    std::for_each(std::execution::par_unseq, v.begin(), v.end(), [](Y& y){ y.increment(); });
}

*/
