#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include "boost/asio.hpp"
#include<vector>

const size_t BUFFER_SIZE = 1024;

class ClientSession
{
public:
    ClientSession(boost::asio::io_service& io_service);
    ~ClientSession();

    boost::asio::ip::tcp::socket& getSocket();

    void start();
    void stop();

private:
    void do_read_();
    void read_handler_(const boost::system::error_code& error, std::size_t bytes_transferred);

    void do_write_(const std::string& buffer);
    void write_handler_(const boost::system::error_code& error, std::size_t bytes_transferred);

    boost::asio::ip::tcp::socket socket_;
    char buffer_[1024];
    // DBHandler
};

std::string get_string_from_streambuf(boost::asio::streambuf& buffer);

#endif /*CLIENT_SESSION_H*/