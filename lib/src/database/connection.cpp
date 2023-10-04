#include "connection.h"

#include <memory>
#include <iostream>
#include <fstream>

#include <boost/format.hpp>
#include <nlohmann/json.hpp>

// DBConnectionOption
std::string DBConnectionOption::getConnectionInfo() const
{
    auto connection_info = boost::format("user=%1% host=%2% port=%3% password=%4% dbname=%5%") % _user % _host % _port % _password % _dbname;
    return connection_info.str();
};

// DBConnection
size_t DBConnection::connect(const DBConnectionOption &connection_option)
{
    try
    {
        connection_ = std::make_shared<pqxx::connection>(connection_option.getConnectionInfo());
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Connection is failed!" << std::endl;
        return EXIT_FAILURE;
    }
};

size_t DBConnection::disconnect()
{
    try
    {
        connection_->disconnect();
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Connection is already closed!" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
};

std::shared_ptr<pqxx::connection> DBConnection::getConnection() { return connection_; };
;

pqxx::result DBConnection::doRequest(const std::string &request)
{
    pqxx::result result;
    try
    {
        pqxx::work transaction{*connection_};
        result = transaction.exec(request);
        transaction.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Request cannot be executed!" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    return result;
};