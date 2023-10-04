#ifndef SESSION_H
#define SESSION_H

#include "boost/asio.hpp"

#include "api.h"

#include <vector>

const size_t DEFAULT_BUFFER_SIZE = 1024;

class Buffer
{
public:
    Buffer(const size_t &buffer_size = DEFAULT_BUFFER_SIZE);
    std::vector<char> &fill();
    std::string get() const;

private:
    std::vector<char> buffer_;
    size_t size_;
};

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::shared_ptr<DBBackend> db_backend, std::shared_ptr<JWTAuthenticationManager> jwt_manager);

    void start(std::shared_ptr<std::vector<std::shared_ptr<Session>>> sessions);
    void stop();

private:
    void do_read_();
    void read_handler_(const boost::system::error_code &error, std::size_t bytes_transferred);

    void do_write_(const std::string &answer_buffer);
    void write_handler_(const boost::system::error_code &error, std::size_t bytes_transferred);

    Buffer buffer_;

    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<RequestAPI> api_;
    std::shared_ptr<std::vector<std::shared_ptr<Session>>> sessions_;
};

#endif /*SESSION_H*/