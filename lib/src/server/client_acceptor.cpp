#include "client_acceptor.h"
#include "boost/bind/bind.hpp"
#include <iostream>

// ClientAcceptor
ClientAcceptor::ClientAcceptor(std::shared_ptr<DBBackend> backend, boost::asio::io_service& io_service, const short& port) : 
    backend_(backend),
    io_service_(io_service),
    acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    sessions_(std::make_shared<std::vector<std::shared_ptr<ClientSession>>>()){};

ClientAcceptor::~ClientAcceptor(){
    this->stop();
    this->acceptor_.cancel();
};

void ClientAcceptor::start(){
    this->do_accept_();
    io_service_.run();
};

void ClientAcceptor::stop(){
    this->acceptor_.close();
};

void ClientAcceptor::do_accept_(){
    std::shared_ptr<ClientSession> new_session = std::make_shared<ClientSession>(this->io_service_);
    
    this->acceptor_.async_accept(
        new_session->getSocket(),
        boost::bind(&ClientAcceptor::accept_handler_, this, new_session, boost::asio::placeholders::error));
};

void ClientAcceptor::accept_handler_(std::shared_ptr<ClientSession> new_session, const boost::system::error_code& error){
    if(!error){
        sessions_->push_back(new_session);
        new_session->start(backend_, sessions_);
    }
    
    this->do_accept_();
};