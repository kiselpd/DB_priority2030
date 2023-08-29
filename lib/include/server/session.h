#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include "boost/asio.hpp"
#include "backend.h"

#include <vector>

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(boost::asio::io_service& io_service);

    boost::asio::ip::tcp::socket& getSocket();

    void start(std::shared_ptr<DBBackend> db, std::shared_ptr<std::vector<std::shared_ptr<ClientSession>>> sessions);
    void stop();

private:
    void do_read_();
    void read_handler_(const boost::system::error_code& error, std::size_t bytes_transferred);

    void do_write_(const std::string& buffer);
    void write_handler_(const boost::system::error_code& error, std::size_t bytes_transferred);

    boost::asio::ip::tcp::socket socket_;
    std::vector<char> buffer_;
    const size_t buffer_size_ = 1024;  

    std::shared_ptr<DBBackend> db_;
    std::shared_ptr<std::vector<std::shared_ptr<ClientSession>>> sessions_;
};

std::string get_string_from_vector(const std::vector<char>& buffer);

#endif /*CLIENT_SESSION_H*/