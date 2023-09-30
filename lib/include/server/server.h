#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <libconfig.h++>

#include "acceptor_service.h"
#include "session_service.h"

#include "backend.h"
class ServerOption
{
public:
    size_t readConfig(const std::string &config_name);

    std::string getJWTSecret() const;
    jwt::params::param_seq_list_t getJWTAlgo() const;

    short getAcceptorPort() const;

    size_t getDBPoolSize() const;
    DBConnectionOption getDBOption() const;

private:
    bool readDBConfig_(const libconfig::Setting& settings);
    bool readJWTConfig_(const libconfig::Setting& settings);
    bool readAcceptorConfig_(const libconfig::Setting& settings);

    int acceptor_port_;

    unsigned int db_pool_size_;
    DBConnectionOption db_option_;

    std::string secret_;
    jwt::params::param_seq_list_t algo_;
};
class Server
{
public:
    Server();

    size_t start(const ServerOption &option);
    size_t stop();

private:
    size_t addAcceptorService_(const ServerOption &option);
    size_t addSessionService_(const ServerOption &option);

    void bindServices_();
    std::shared_ptr<DBBackend> connectToDB_(const DBConnectionOption &db_option, const size_t &db_pool_size);

    std::shared_ptr<boost::asio::io_service> io_;

    std::shared_ptr<AcceptorService> acceptor_service_;
    std::shared_ptr<SessionService> session_service_;
};

#endif /*SERVER_H*/