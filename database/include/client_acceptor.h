#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

#include "boost/asio.hpp"

class ClientAcceptor
{
public:
    ClientAcceptor(const short& port);
    ~ClientAcceptor();

    void start();
    void stop();
private:
    void do_accept_();
    void accept_handler_(const boost::system::error_code& error);

    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};

#endif /*CLIENT_ACCEPTOR_H*/