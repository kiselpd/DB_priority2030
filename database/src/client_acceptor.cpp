#include "client_acceptor.h"
#include "client_session.h"

// ClientAcceptor
ClientAcceptor::ClientAcceptor(const short& port) : 
    acceptor_(this->io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), socket_(this->io_service_) {};

ClientAcceptor::~ClientAcceptor(){
    this->stop();
    this->acceptor_.cancel();
};

void ClientAcceptor::start(){
    this->do_accept_();
    this->io_service_.run();
};

void ClientAcceptor::stop(){
    this->acceptor_.close();
    this->io_service_.stop();
};

void ClientAcceptor::do_accept_(){
    auto handler = [this](const boost::system::error_code& error) {
        this->accept_handler_(error);
    };

    this->acceptor_.async_accept(
        this->socket_,
        handler);
};

void ClientAcceptor::accept_handler_(const boost::system::error_code& error){
    if(!error){
        auto new_session = std::make_shared<ClientSession>(std::move(this->socket_));
        new_session->start();
    }
    
    this->do_accept_();
};