#include <chrono>
#include <iostream>
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
