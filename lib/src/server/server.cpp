#include "server.h"

// ServerOption
size_t ServerOption::readConfig(const std::string &config_name)
{
    try
    {
        libconfig::Config cfg;
        cfg.readFile(config_name.c_str());
        const auto &root = cfg.getRoot();

        if (readAcceptorConfig_(root["application"]["acceptor"]) && readDBConfig_(root["application"]["database"]) && readJWTConfig_(root["application"]["jwt"]))
            return EXIT_SUCCESS;
        else
            return EXIT_FAILURE;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
};

std::string ServerOption::getJWTSecret() const
{
    return this->secret_;
};

jwt::params::param_seq_list_t ServerOption::getJWTAlgo() const
{
    return algo_;
};

short ServerOption::getAcceptorPort() const
{
    return this->acceptor_port_;
};

size_t ServerOption::getDBPoolSize() const
{
    return this->db_pool_size_;
};

DBConnectionOption ServerOption::getDBOption() const
{
    return this->db_option_;
};

bool ServerOption::readDBConfig_(const libconfig::Setting &settings)
{
    return settings.lookupValue("pool_size", db_pool_size_) &&
           settings.lookupValue("name", db_option_._dbname) &&
           settings.lookupValue("host", db_option_._host) &&
           settings.lookupValue("password", db_option_._password) &&
           settings.lookupValue("port", db_option_._port) &&
           settings.lookupValue("user", db_option_._user);
};

bool ServerOption::readJWTConfig_(const libconfig::Setting &settings)
{
    algo_ = {"HS256"};
    return settings.lookupValue("secret", secret_);
};

bool ServerOption::readAcceptorConfig_(const libconfig::Setting &settings)
{
    return settings.lookupValue("port", acceptor_port_);
};

// Server
Server::Server() : io_(std::make_shared<boost::asio::io_service>()) {}

size_t Server::start(const ServerOption &option)
{
    auto error = this->addAcceptorService_(option);

    if (!error)
        error = this->addSessionService_(option);

    if (!error)
    {
        this->bindServices_();
        acceptor_service_->start();
        io_->run();
    }

    return error;
};

size_t Server::stop()
{
    acceptor_service_->stop();
    io_->stop();
};

size_t Server::addAcceptorService_(const ServerOption &option)
{
    try
    {
        acceptor_service_ = std::make_shared<AcceptorService>(io_, option.getAcceptorPort());
        std::cerr << "Acceptor service is ready!" << '\n';

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "Not possible to use port " << option.getAcceptorPort() << "." << '\n';
        return EXIT_FAILURE;
    }
};

size_t Server::addSessionService_(const ServerOption &option)
{
    try
    {
        auto db = this->connectToDB_(option.getDBOption(), option.getDBPoolSize());

        if (db)
        {
            auto jwt_manager = std::make_shared<JWTAuthenticationManager>(io_, option.getJWTAlgo(), option.getJWTSecret());
            session_service_ = std::make_shared<SessionService>(db, jwt_manager);
            std::cerr << "Session service is ready!" << '\n';
        }
        else
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
};

std::shared_ptr<DBBackend> Server::connectToDB_(const DBConnectionOption &db_option, const size_t &db_pool_size)
{
    std::shared_ptr<DBBackend> db_;
    auto conn_pool = std::make_shared<DBConnectionPool>(db_option, db_pool_size);
    auto pool_size = conn_pool->createPool();

    if (pool_size)
    {
        std::cout << "Pool is created with " << pool_size << " connections." << std::endl;
        db_ = std::make_shared<DBBackend>(conn_pool);
    }
    else
        std::cout << "Pool is not created." << std::endl;

    return db_;
};

void Server::bindServices_()
{
    auto mediator = std::make_shared<Mediator>(acceptor_service_, session_service_);
    mediator->registerColleagues();
};