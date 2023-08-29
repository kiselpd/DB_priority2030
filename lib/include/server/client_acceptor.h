#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

#include "boost/asio.hpp"
#include "session.h"
#include "backend.h"

class ClientAcceptor
{
public:
    ClientAcceptor(std::shared_ptr<DBBackend> backend, boost::asio::io_service& io_service, const short& port);
    ~ClientAcceptor();

    void start();
    void stop();
private:
    void do_accept_();
    void accept_handler_(ClientSession* new_session, const boost::system::error_code& error);

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<DBBackend> backend_;
};

#endif /*CLIENT_ACCEPTOR_H*/