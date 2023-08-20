#include "client_session.h"
#include "boost/asio/buffer.hpp"
#include <iostream>

// ClientSession
ClientSession::ClientSession(boost::asio::io_service& io_service) : socket_(io_service){
};

ClientSession::~ClientSession(){
    this->stop();
    this->socket_.cancel();
};

boost::asio::ip::tcp::socket& ClientSession::getSocket(){
    return socket_;
};

void ClientSession::start(std::shared_ptr<DBBackend> db){
    db_ = db;
    this->do_read_();
};

void ClientSession::stop(){
    this->socket_.close();
};

void ClientSession::do_read_(){
    auto handler = [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
        this->read_handler_(error, bytes_transferred);
    };
    buffer_.clear();
    buffer_.resize(this->buffer_size_);
    this->socket_.async_read_some(
        boost::asio::buffer(buffer_),
        handler
    );
};

void ClientSession::read_handler_(const boost::system::error_code& error, size_t bytes_transferred){
    if(!error){
        std::cout << get_string_from_vector(this->buffer_);
        auto answer = db_->doRequest(get_string_from_vector(this->buffer_));
        this->do_write_(answer);
    }
    //обработать ошибку
};

void ClientSession::do_write_(const std::string& buffer){
    auto handler = [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
        this->write_handler_(error, bytes_transferred);
    };

    this->socket_.async_write_some(
        boost::asio::buffer(buffer),
        handler
    );
};

void ClientSession::write_handler_(const boost::system::error_code& error, std::size_t bytes_transferred){
    if(!error)
        this->do_read_();

    //обработать ошибку
};

std::string get_string_from_vector(const std::vector<char>& buffer){
    std::string new_str(buffer.begin(), buffer.end());
    return new_str;
};