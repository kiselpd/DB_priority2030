#include "session.h"
#include <iostream>
#include "json_builder.h"
#include "answer.h"

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
        std::string str_request = get_string_from_vector(this->buffer_);
        std::shared_ptr<BaseBuilder> builder;
        
        std::string type = getType(str_request);
        std::string body = getBody(str_request);

        if(type == "GET")
            builder = std::make_shared<BuilderSelectRequest>(body);
        if(type == "POST")
            builder = std::make_shared<BuilderUpdateRequest>(body);
        if(type == "PUT")
            builder = std::make_shared<BuilderInsertRequest>(body);
        if(type == "DELETE")
            builder = std::make_shared<BuilderDeleteRequest>(body);

        auto request = builder->build();
        auto result = db_->doRequest(request);
        Answer answer(result);
        
        do_write_(answer.getAnswer());
    }
    else
        int a;
        // delete this;
    
    
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