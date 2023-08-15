#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

#include "boost/asio.hpp"
#include "client_session.h"

class ClientAcceptor
{
public:
    ClientAcceptor(boost::asio::io_service& io_service, const short& port);
    ~ClientAcceptor();

    void start();
    void stop();
private:
    void do_accept_();
    void accept_handler_(ClientSession* new_session, const boost::system::error_code& error);

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif /*CLIENT_ACCEPTOR_H*/