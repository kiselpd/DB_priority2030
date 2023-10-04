#include "session.h"

#include <iostream>

// Buffer
Buffer::Buffer(const size_t &buffer_size) : size_(buffer_size)
{
    buffer_.resize(this->size_);
};

std::vector<char> &Buffer::fill()
{
    std::fill(buffer_.begin(), buffer_.end(), 0);    
    return buffer_;
};

std::string Buffer::get() const
{
    return std::string(buffer_.begin(), buffer_.end());
};

// Session
Session::Session(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                 std::shared_ptr<DBBackend> db_backend,
                 std::shared_ptr<JWTAuthenticationManager> jwt_manager) : socket_(socket),
                                                                          api_(std::make_shared<RequestAPI>(db_backend, jwt_manager)){};

void Session::start(std::shared_ptr<std::vector<std::shared_ptr<Session>>> sessions)
{
    sessions_ = sessions;
    this->do_read_();
};

void Session::stop()
{
    socket_->close();
    auto self = this->shared_from_this();
    auto it = std::find(sessions_->begin(), sessions_->end(), self);
    sessions_->erase(it);
};

void Session::do_read_()
{
    auto handler = [this](const boost::system::error_code &error, std::size_t bytes_transferred)
    {
        this->read_handler_(error, bytes_transferred);
    };

    socket_->async_read_some(
        boost::asio::buffer(buffer_.fill()),
        handler);
};

void Session::read_handler_(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error)
        this->do_write_(api_->doRequest(buffer_.get()));
    else
        this->stop();
};

void Session::do_write_(const std::string &answer_buffer)
{
    auto handler = [this](const boost::system::error_code &error, std::size_t bytes_transferred)
    {
        this->write_handler_(error, bytes_transferred);
    };

    socket_->async_write_some(
        boost::asio::buffer(answer_buffer),
        handler);
};

void Session::write_handler_(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error)
        this->do_read_();
    else
        this->stop();
};