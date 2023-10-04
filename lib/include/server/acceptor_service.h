#ifndef ACCEPTOR_SERVICE_H
#define ACCEPTOR_SERVICE_H

#include "mediator.h"

#include "boost/asio.hpp"

class AcceptorService : public AcceptorServiceColleague
{
public:
    AcceptorService(std::shared_ptr<boost::asio::io_service> io_service, const short &port);
    ~AcceptorService();

    void start();
    void stop();

private:
    void do_accept_();
    void accept_handler_(const boost::system::error_code &error);

    std::shared_ptr<boost::asio::io_service> io_service_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;

    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
};

#endif /*ACCEPTOR_SERVICE_H*/