#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include "acceptor_service.h"
#include "session_service.h"

#include "backend.h"
class ServerOption
{
public:
    size_t readConfig(const std::string &config_name);

    std::string getSecret() const;
    short getAcceptorPort() const;
    size_t getDBPoolSize() const;
    DBConnectionOption getDBOption() const;

private:
    std::string secret_;
    short accept_port_;

    size_t db_pool_size_;
    DBConnectionOption db_option_;
};
class Server
{
public:
    Server();

    size_t autoRun(const ServerOption &option);

    size_t addAcceptorService(const short &port);
    size_t addSessionService(const DBConnectionOption &db_option, const size_t &db_pool_size, const std::string &secret_jwt_word);

    size_t start();
    size_t stop();

private:
    void bindServices_();
    std::shared_ptr<DBBackend> connectToDB_(const DBConnectionOption &db_option, const size_t &db_pool_size);

    std::shared_ptr<boost::asio::io_service> io_;

    std::shared_ptr<AcceptorService> acceptor_service_;
    std::shared_ptr<SessionService> session_service_;
};

#endif /*SERVER_H*/