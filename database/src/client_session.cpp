#include "client_session.h"
#include <iostream>

// class ClientSession
// {
// public:
//     ClientSession(boost::asio::ip::tcp::socket socket);
//     ~ClientSession();

//     void start();
//     void stop();

// private:
//     void do_read_();
//     void read_handler_(const boost::system::error_code& error);

//     void do_write_();
//     void write_handler_(const boost::system::error_code& error);

//     boost::asio::ip::tcp::socket socket_;
//     boost::asio::streambuf buffer_;
//     // DBHandler
// };
// ClientSession
ClientSession::ClientSession(boost::asio::ip::tcp::socket socket) :
    socket_(std::move(socket)) {};

ClientSession::~ClientSession(){
    this->stop();
    this->socket_.cancel();
};

void ClientSession::start(){
    this->do_read_();
};

void ClientSession::stop(){
    this->socket_.close();
};

void ClientSession::do_read_(){
    auto handler = [this](const boost::system::error_code& error) {
        this->read_handler_(error);
    };

    this->socket_.async_read_some(
        this->buffer_,
        handler
    );
};

void ClientSession::read_handler_(const boost::system::error_code& error){
    if(!error){
        //засунуть буффер в запрос и отправить в бд
        //получить ответ и отправить его
        std::cout << "SomethingResive" << std::endl;
        std::string answer = "GOOD!"; //костыль доделать
        this->do_write_(answer);
    }

    //обработать ошибку
};

void ClientSession::do_write_(const std::string& buffer){
    auto handler = [this](const boost::system::error_code& error) {
        this->write_handler_(error);
    };

    this->socket_.async_write_some(
        boost::asio::buffer(buffer),
        handler
    );
};

void ClientSession::write_handler_(const boost::system::error_code& error){
    if(!error)
        this->do_read_();

    //обработать ошибку
};


