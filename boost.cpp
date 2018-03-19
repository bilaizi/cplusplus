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
