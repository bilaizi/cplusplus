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
