#include "server.h"

// class ServerOption
// {
// public:
//     size_t readConfig(const std::string &config_name);

//     std::string getSecret() const;
//     short getAcceptorPort() const;
//     size_t getDBPoolSize() const;
//     DBConnectionOption getDBOption() const;

// private:
//     std::string secret_;
//     short accept_port_;

//     size_t db_pool_size_;
//     DBConnectionOption db_option_;
// };
// ServerOption

// Server
Server::Server() : io_(std::make_shared<boost::asio::io_service>()) {}

size_t Server::autoRun(const ServerOption &option)
{
    auto error = this->addAcceptorService(option.getAcceptorPort());

    if (!error)
        error = this->addSessionService(option.getDBOption(), option.getDBPoolSize(), option.getSecret());

    if (!error)
        error = this->start();

    return error;
};

size_t Server::addAcceptorService(const short &port)
{
    try
    {
        acceptor_service_ = std::make_shared<AcceptorService>(io_, port);
        std::cerr << "Acceptor service is ready!" << '\n';

        if (acceptor_service_ && session_service_)
            this->bindServices_();

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "Not possible to use port " << port << "." << '\n';
        return EXIT_FAILURE;
    }
};

size_t Server::addSessionService(const DBConnectionOption &db_option, const size_t &db_pool_size, const std::string &secret_jwt_word)
{
    try
    {
        auto db = this->connectToDB_(db_option, db_pool_size);

        if (db)
        {
            auto jwt_manager = std::make_shared<JWTAuthenticationManager>(io_, jwt::params::param_seq_list_t({"HS256"}), secret_jwt_word);
            session_service_ = std::make_shared<SessionService>(db, jwt_manager);
            std::cerr << "Session service is ready!" << '\n';
        }

        if (acceptor_service_ && session_service_)
            this->bindServices_();

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
};

size_t Server::start()
{
    acceptor_service_->start();
    io_->run();
};

size_t Server::stop()
{
    io_->stop();
};

std::shared_ptr<DBBackend> Server::connectToDB_(const DBConnectionOption &db_option, const size_t &db_pool_size)
{
    std::shared_ptr<DBBackend> db_;
    auto conn_pool = std::make_shared<DBConnectionPool>(db_option, db_pool_size);
    auto pool_size = conn_pool->createPool();

    if (pool_size)
    {
        std::cout << "Pool is created with " << pool_size << " connections." << std::endl;
        auto db_ = std::make_shared<DBBackend>(conn_pool);
    }
    else
        std::cout << "Pool is not created." << std::endl;

    return db_;
};

void Server::bindServices_()
{
    auto mediator = std::make_shared<Mediator>(acceptor_service_, session_service_);
};