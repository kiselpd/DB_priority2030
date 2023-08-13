#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include "boost/asio.hpp"

class ClientSession
{
public:
    ClientSession(boost::asio::ip::tcp::socket socket);
    ~ClientSession();

    void start();
    void stop();

private:
    void do_read_();
    void read_handler_(const boost::system::error_code& error);

    void do_write_(const std::string& buffer);
    void write_handler_(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer_;
    // DBHandler
};

#endif /*CLIENT_SESSION_H*/