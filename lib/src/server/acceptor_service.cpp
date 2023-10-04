#include "acceptor_service.h"

#include "boost/bind/bind.hpp"

// ClientAcceptor
AcceptorService::AcceptorService(std::shared_ptr<boost::asio::io_service> io_service, const short &port) : io_service_(io_service),
                                                                                                           acceptor_(std::make_shared<boost::asio::ip::tcp::acceptor>(*io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))){};

AcceptorService::~AcceptorService()
{
    this->stop();
    acceptor_->cancel();
};

void AcceptorService::start()
{
    this->do_accept_();
};

void AcceptorService::stop()
{
    acceptor_->close();
};

void AcceptorService::do_accept_()
{
    socket_ = std::make_shared<boost::asio::ip::tcp::socket>(*io_service_);

    acceptor_->async_accept(
        *socket_,
        boost::bind(&AcceptorService::accept_handler_, this, boost::asio::placeholders::error));
};

void AcceptorService::accept_handler_(const boost::system::error_code &error)
{
    if (!error)
        this->send(socket_);

    socket_.reset();
    this->do_accept_();
};