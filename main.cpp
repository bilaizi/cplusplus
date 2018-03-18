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

// 


/* 
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
atomic<bool> initialized(false);
mutex mut;

void func(){
    if(!initialized) {
        unique_lock lock(mut);
        if (!initialized) {
            initializeSharedResources();
	    initialized = true;
	}
    }
    cout << "OK" << endl;
}

int main(){
    vector<thread> threads;
    for (int i = 0; i < 5; ++i) 
	threads.push_back(thread{ func });
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
