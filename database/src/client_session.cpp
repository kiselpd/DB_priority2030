#include "client_session.h"
#include <iostream>

// ClientSession
ClientSession::ClientSession(boost::asio::io_service& io_service) : socket_(io_service){};

ClientSession::~ClientSession(){
    this->stop();
    this->socket_.cancel();
};

boost::asio::ip::tcp::socket& ClientSession::getSocket(){
    return socket_;
};

void ClientSession::start(){
    this->do_read_();
};

void ClientSession::stop(){
    this->socket_.close();
};

void ClientSession::do_read_(){
    auto handler = [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
        this->read_handler_(error, bytes_transferred);
    };

    this->socket_.async_read_some(
        boost::asio::buffer(this->buffer_),
        handler
    );
};

void ClientSession::read_handler_(const boost::system::error_code& error, size_t bytes_transferred){
    if(!error){
        // std::string request = get_string_from_streambuf(this->buffer_);

        std::cout << buffer_ << std::endl;
        std::string answer = "GOOD!"; //костыль доделать
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

std::string get_string_from_streambuf(boost::asio::streambuf& buffer){
    std::ostringstream stream_buffer;
	stream_buffer<<&buffer;
    std::string new_str = stream_buffer.str();
    return new_str;
};