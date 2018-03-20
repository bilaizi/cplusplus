#include <algorithm> 
#include <array>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

using namespace std;

auto main()->int{
    array<int, 3> values = { 11, 22, 33 };
    auto [x, y, z] = values;
    struct Point { double x, y, z; };
    Point point={1.0, 2.0, 3.0};
    auto [x, y, z] = point;
    int* a = nullptr;
    a= new int; 
    if(!a){
    }
    delete a;
    a = nullptr;
    auto b= make_unique<int>();
    unique_ptr<int> c{new int};
    auto d= make_unique<int[]>(10);
    unique_ptr<int[]> e{new int[10]};
    auto f= make_shared<int>();
    shared_ptr<int[]> g{new int[10]};
    const int versionNumberMajor = 2;
    const int versionNumberMinor = 1;
    const std::string productName = "Super Hyper Net Modulator";
	

    
    cout  << __func__ << " " << "Hello, Wandbox!" << endl;
    return 0;
}
struct bad_date : std::exception {
   char const * what() const noexcept override { 
       return "bad_date"; 
   }
};

struct month {
    constexpr month(int m) noexcept : value{m} {}
    int value;
};
static constexpr month jan{1};
static constexpr month feb{2};
static constexpr month mar{3};
static constexpr month apr{4};
static constexpr month may{5};
static constexpr month jun{6};
static constexpr month jul{7};
static constexpr month aug{8};
static constexpr month sep{9};
static constexpr month oct{10};
static constexpr month nov{11};
static constexpr month dec{12};

struct year {
    constexpr year(int y) noexcept : value{y} {}
    int value;
};

// function template
template <typename T>
void Swap(T& a, T& b){
    T c;
    c = b;
    b = a;
    a = c;
}
// class template
template <typename T>
class Array{
protected:
    T* arr;
public:
    Array(size_t s = 10){
        arr = new T[s];
    }
    ~Array() {
        delete [] arr;
    }
    void Set(size_t index, T input){
        arr[index] = input;
    }
    T& Get(size_t index){
        return arr[index];
    }
};
// 


/* 
auto get_widget(int id)-> shared_ptr<widget> {
    static map<int, weak_ptr<widget>> cache;
    static mutex mut_cache;
    lock_guard hold{ mut_cache };
    auto sp = cache[id].lock();
    if(!sp) 
      cache[id] = sp = load_widget(id);
    return sp;
}

#include <thread>
#include <iostream>
using namespace std;

class Request{
public:
    Request(int id) : mId{ id }{ }
    void process(){
	cout << "Processing request " << mId << endl;
    }
private:
    int mId;
};
int main(){
    Request req{ 100 };
    thread t{ &Request::process, &req };
    t.join();
    return 0;
}


#include <iostream>
#include <thread>
using namespace std;
class Counter{
public:
    Counter(int id, int numIterations)
	: mId{ id }, mNumIterations{ numIterations }{
    }
    void operator()() const{
	for(int i = 0; i < mNumIterations; ++i) {
	    cout << "Counter " << mId << " has value " << i << endl;
	}
    }
private:
    int mId;
    int mNumIterations;
};
int main(){
    // Using uniform initialization syntax
    thread t1{ Counter{ 1, 20 } };
    // Using named variable
    Counter c{2, 12};
    thread t2{c};
    // Using temporary
    thread t3{Counter{3, 10}};
    // Wait for threads to finish
    t1.join();
    t2.join();
    t3.join();
    return 0;
}


#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>
using namespace std;

void doSomeWork(){
    for (int i = 0; i < 5; ++i) 
	cout << i << endl;
    cout << "Thread throwing a runtime_error exception..." << endl;
    throw runtime_error("Exception from thread");
}
void threadFunc(exception_ptr& err){
    try {
        doSomeWork();
    } catch (...) {
	cout << "Thread caught exception, returning exception..." << endl;
	err = current_exception();
    }
}
void doWorkInThread(){
    exception_ptr error;
    // Launch thread
    thread t{ threadFunc, ref(error) };
    // Wait for thread to finish
    t.join();
    // See if thread has thrown any exception
    if (error) {
	cout << "Main thread received exception, rethrowing it..." << endl;
	rethrow_exception(error);
    } else {
	cout << "Main thread did not receive any exception." << endl;
    }
}
int main(){
    try {
	doWorkInThread();
    } catch (const exception& e) {
	cout << "Main function caught: '" << e.what() << "'" << endl;
    }
    return 0;
}

#include <atomic>
#include <iostream>
#include <type_traits>
#include <utility>

struct A { int a[100]; };
struct B { int x, y; };
int main(){
    std::cout << std::boolalpha 
        << std::is_trivially_copyable_v<A> << " " 
        << "std::atomic<A> is lock free? "
        << std::atomic<A>{}.is_lock_free() << '\n'
        << std::is_trivially_copyable_v<B> << " " 
        << "std::atomic<B> is lock free? "
        << std::atomic<B>{}.is_lock_free() << '\n';
}
//true std::atomic<A> is lock free? false
//true std::atomic<B> is lock free? true

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;
using namespace std::chrono_literals;

void increment(atomic<int>& counter){
	for (int i = 0; i < 100; ++i) {
		++counter;
		this_thread::sleep_for(1ms);
	}
}
//optimal
void increment(atomic<int>& counter){
    int result = 0;
    for(int i = 0; i < 100; ++i) {
	++result;
	this_thread::sleep_for(1ms);
    }
    counter += result;
}
int main(){
    atomic<int> counter{};
    vector<thread> threads;
    for (int i = 0; i < 10; ++i) 
	threads.push_back(thread{ increment, ref(counter) });//threads.emplace_back(func, ref(counter));
    for (auto& t : threads) 
	t.join();
    cout << "Result = " << counter << endl;
    atomic<int> value{10};
    cout << "Value = " << value << endl;
    int fetched = value.fetch_add(4);
    cout << "Fetched = " << fetched << endl;
    cout << "Value = " << value << endl;
    return 0;
}

#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

class Counter{
public:
    Counter(int id, int numIterations)
	: mId{ id }, mNumIterations{ numIterations }{
    }
    void operator()() const{
	for(int i = 0; i < mNumIterations; ++i) {
	    lock_guard lock(mMutex);
	    cout << "Counter " << mId << " has value " << i << endl;
	}
    }
private:
    int mId;
    int mNumIterations;
    static mutex mMutex;
};
mutex Counter::mMutex;

int main(){
    // Using uniform initialization syntax
    thread t1{ Counter{ 1, 20 } };
    // Using named variable
    Counter c{2, 12};
    thread t2{c};
    // Using temporary
    thread t3{Counter{3, 10}};
    // Wait for threads to finish
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
//t1 0~19 t2 0~11 t3 0~9


#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

class Counter{
public:
    Counter(int id, int numIterations)
	: mId{ id }, mNumIterations{ numIterations }{
    }
    void operator()() const{
	for(int i = 0; i < mNumIterations; ++i) {
	    using namespace std::chrono_literals;
	    unique_lock lock{mTimedMutex, 200ms};
	    if(lock) {
		cout << "Counter " << mId << " has value " << i << endl;
	    } else {
		// Lock not acquired in 200 ms
	    }
	}
    }
private:
    int mId;
    int mNumIterations;
    static timed_mutex mTimedMutex;
};
timed_mutex Counter::mTimedMutex;
int main(){
    // Using uniform initialization syntax
    thread t1{ Counter{ 1, 20 } };
    // Using named variable
    Counter c{2, 12};
    thread t2{c};
    // Using temporary
    thread t3{Counter{3, 10}};
    // Wait for threads to finish
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
threads.emplace_back(processingFunction);
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

void initializeSharedResources(){
    // ... Initialize shared resources that will be used by multiple threads.
    cout << "Shared resources initialized." << endl;
}
atomic<bool> gInitialized(false);
mutex gMutex;

void processingFunction(){
    if (!gInitialized) {
	unique_lock lock(gMutex);  // C++17
	if (!gInitialized) {
	    initializeSharedResources();
	    gInitialized = true;
	}
    }
    cout << "OK" << endl;
}
int main(){
    vector<thread> threads;
    for (int i = 0; i < 5; ++i) {
	threads.push_back(thread{ processingFunction });//threads.emplace_back(processingFunction);
    }
    for (auto& t : threads) 
	t.join();
    return 0;
}


#include <mutex>

std::mutex mut1;
std::mutex mut2;
void process(){
    //std::scoped_lock locks{mut1, mut2};
   std::scoped_lock<std::mutex, std::mutex> locks{mut1, mut2};
    // Locks acquired
} // Locks automatically released

int main(){
    process();
    return 0;
}

#include <mutex>
#include <thread>
#include <chrono>
 
struct Account {
    explicit Account(int balance) :balance{balance} {}
    double balance;
    std::mutex m;
};
 
void transfer(Account &from,Account &to, int amount){
    // don't actually take the locks yet
    std::unique_lock lock1(from.m, std::defer_lock);// C++17
    std::unique_locklock2(to.m, std::defer_lock);// C++17
    // lock both unique_locks without deadlock
    std::lock(lock1, lock2);
    from.balance -= amount;
    to.balance += amount;
    // 'from.m' and 'to.m' mutexes unlocked in 'unique_lock' dtors
}
 void transfer(Account &from,Account &to, int amount){
    // lock both mutexes without deadlock
    std::lock(from.m, to.m);
    // make sure both already-locked mutexes are unlocked at the end of scope
    std::lock_guard lock1(from.m, std::adopt_lock);// C++17
    std::lock_guar lock2(to.m, std::adopt_lock);// C++17
    from.balance -= amount;
    to.balance += amount;
    // 'from.m' and 'to.m' mutexes unlocked in 'unique_lock' dtors
}
int main(){
    Account a{100};
    Account b{50};
    std::thread t1(transfer, std::ref(a), std::ref(b), 10);
    std::thread t2(transfer, std::ref(b), std::ref(a), 5);
    t1.join();
    t2.join();
}

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
 
struct Employee {
    Employee(std::string id) : id{id} {}
    std::string id;
    std::vector<std::string> lunch_partners;
    std::mutex m;
    std::string output() const{
        std::string ret = "Employee " + id + " has lunch partners: ";
        for(const auto& partner : lunch_partners)
            ret += partner + " ";
        return ret;
    }
};
 
void send_mail(Employee &, Employee &){
    // simulate a time-consuming messaging operation
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
 
void assign_lunch_partner(Employee &e1, Employee &e2){
    static std::mutex io_mutex;
    {
        std::lock_guard lk(io_mutex);// C++17
        std::cout << e1.id << " and " << e2.id << " are waiting for locks" << std::endl;
    }
    {
        // use std::scoped_lock to acquire two locks without worrying about 
        // other calls to assign_lunch_partner deadlocking us
        // and it also provides a convenient RAII-style mechanism
        std::scoped_lock lock(e1.m, e2.m);
 
        // Equivalent code 1 (using std::lock and std::lock_guard)
        // std::lock(e1.m, e2.m);
        // std::lock_guard lk1(e1.m, std::adopt_lock);// C++17
        // std::lock_guard lk2(e2.m, std::adopt_lock);// C++17
 
        // Equivalent code 2 (if unique_locks are needed, e.g. for condition variables)
        // std::unique_lock lk1(e1.m, std::defer_lock);// C++17
        // std::unique_lock lk2(e2.m, std::defer_lock);// C++17
        // std::lock(lk1, lk2);
        {
            std::lock_guard lk(io_mutex);// C++17
            std::cout << e1.id << " and " << e2.id << " got locks" << std::endl;
        }
        e1.lunch_partners.push_back(e2.id);
        e2.lunch_partners.push_back(e1.id);
    }
    send_mail(e1, e2);
    send_mail(e2, e1);
}
int main(){
    Employee alice("alice"), bob("bob"), christina("christina"), dave("dave");
    // assign in parallel threads because mailing users about lunch assignments takes a long time
    std::vector<std::thread> threads;
    threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
    threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));
    for(auto &thread : threads) 
        thread.join();
    std::cout << alice.output() << '\n'  << bob.output() << '\n'
              << christina.output() << '\n' << dave.output() << '\n';
}


#include <future>
#include <iostream>

auto main()->int{
    std::promise<void>  t1_ready_promise, t2_ready_promise;
    std::promise<int> ready_promise;
    auto ready_future = ready_promise.get_future().share();
    //std::shared_future<int> ready_future{ready_promise.get_future()};
    auto f1 = [&t1_ready_promise, ready_future] {
        t1_ready_promise.set_value();
	return ready_future.get();
    };
    auto f2 = [&t2_ready_promise, ready_future] {
	t2_ready_promise.set_value();
	return ready_future.get();
    };
    // Launch two threads.
    auto res1 = std::async(std::launch::async, f1);
    auto res2 = std::async(std::launch::async, f2);
    // wait for the threads to become ready
    t1_ready_promise.get_future().wait();
    t2_ready_promise.get_future().wait();
    // the threads are ready, start the clock
    // signal the threads to go
    ready_promise.set_value(42);
    std::cout << "Thread 1 received the signal " << res1.get() << " \n"
              << "Thread 2 received the signal " << res2.get() << " \n";
}
//Thread 1 received the signal 42 
//Thread 2 received the signal 42 


#include <chrono>
#include <future>
#include <iostream>

auto main()->int{	
    std::promise<void> ready_promise, t1_ready_promise, t2_ready_promise;
    std::shared_future<void> ready_future{ ready_promise.get_future() };
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    auto f1 = [&, ready_future] {
	t1_ready_promise.set_value();
	ready_future.wait(); // waits for the signal from main()
        return std::chrono::high_resolution_clock::now() - start;
    };
    auto f2 = [&, ready_future] {
	t2_ready_promise.set_value();
	ready_future.wait(); // waits for the signal from main()
        return std::chrono::high_resolution_clock::now() - start;
    };
    // Launch two threads.
    auto res1 = std::async(std::launch::async, f1);
    auto res2 = std::async(std::launch::async, f2);
    // wait for the threads to become ready
    t1_ready_promise.get_future().wait();
    t2_ready_promise.get_future().wait();
    // the threads are ready, start the clock
    start = std::chrono::high_resolution_clock::now();
    // signal the threads to go
    ready_promise.set_value();
    std::cout << "Thread 1 received the signal " << res1.get().count() << " ms after start\n"
              << "Thread 2 received the signal " << res2.get().count() << " ms after start\n";
}
//Thread 1 received the signal 378511 ms after start
//Thread 2 received the signal 39401 ms after start


#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <string_view>
#include <thread>

class Logger{
public:
	// Starts a background thread writing log entries to a file.
	Logger();
	// Gracefully shut down background thread.
	virtual ~Logger();
	// Prevent copy construction and assignment.
	Logger(const Logger& other) = delete;
	Logger& operator=(const Logger& rhs) = delete;
	// Add log entry to the queue.
	void log(std::string_view entry);
private:
	// The function running in the background thread.
	void processEntries();
	// Boolean telling the background thread to terminate.
	bool mExit = false;
	// Mutex and condition variable to protect access to the queue.
	std::mutex mMutex;
	std::condition_variable mCondVar;
	std::queue<std::string> mQueue;
	// The background thread.
	std::thread mThread;
};


#include <chrono>
#include <fstream>
#include <iostream>
#include "Logger.h"
using namespace std;

Logger::Logger(){
	// Start background thread.
	mThread = thread{&Logger::processEntries, this};
}

Logger::~Logger(){
    {
	unique_lock lock(mMutex);  // C++17
	// Gracefully shut down the thread by setting mExit to true and notifying the thread.
	mExit = true;
	// Notify condition variable to wake up thread.
	mCondVar.notify_all();
    }
    // Wait until thread is shut down. This should be outside the above code block 
    // because the lock must be released before calling join()!
    mThread.join();
}

void Logger::log(string_view entry){
    // Lock mutex and add entry to the queue.
    unique_lock lock(mMutex);  // C++17
    mQueue.push(string{entry});//mQueue.emplace(entry);
    // Notify condition variable to wake up thread.
    mCondVar.notify_all();
}

void Logger::processEntries(){
    // Open log file.
    ofstream logFile("log.txt");
    if (logFile.fail()) {
	cerr << "Failed to open logfile." << endl;
	return;
    }
    // Start processing loop.
    unique_lock lock(mMutex);  // C++17
    while(true){
	// You can add artificial delays on specific places in your multithreaded
        // code to trigger certain behavior. Note that such delays should only be
	// added for testing, and should be removed from your final code!
	//
	// For example, in this Logger class, you could add the following delay
	// to test that the race-condition with the destructor as discussed
	// in Chapter 23 is solved.
	//this_thread::sleep_for(1000ms); // Needs #include <chrono>

	if(!mExit) { // Only wait for notifications if we don't have to exit.
	     // Wait for a notification.
	     mCondVar.wait(lock);
	}
	// Condition variable is notified, so something might be in the queue and/or we need to shut down this thread.
	lock.unlock();
	while(true){
	    lock.lock();
	    if(mQueue.empty())
		break;
	    else {
		logFile << mQueue.front() << endl;
		mQueue.pop();
	    }
	    lock.unlock();
	}
	if(mExit) 
	    break;
    }
}

#include <thread>
#include <vector>
using namespace std;

void logSomeMessages(int id, Logger& logger){
    for(int i = 0; i < 10; ++i) {
	stringstream ss;
	ss << "Log entry " << i << " from thread " << id;
	logger.log(ss.str());
    }
}

int main(){
    Logger logger;
    vector<thread> threads;
    // Create a few threads all working with the same Logger instance.
    for(int i = 0; i < 10; ++i) {
        threads.emplace_back(logSomeMessages, i, ref(logger));// threads.push_back(thread{ logSomeMessages, i, ref(logger) });
    }
    // Wait for all threads to finish.
    for(auto& t : threads) {
	t.join();
    }
    return 0;
}
*/

/*
auto main()->int{
    return 0;
}
int main(){
    return 0;
}
auto main(int argc,char** argv)->int{
    return 0;
}
int main(int argc,char** argv){
    return 0;
}
*/
