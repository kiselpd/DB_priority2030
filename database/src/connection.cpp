#include "connection.h"
#include <boost/format.hpp>
#include <memory>
#include <iostream>

// DBConnectionOption
DBConnectionOption::DBConnectionOption(const std::string& host, const std::string& port, const std::string& user, const std::string& password, const std::string& name) :
        _host(host), _port(port), _user(user), _password(password), _name(name){};

std::string DBConnectionOption::getConnectionInfo() const{
    auto connection_info = boost::format("user=%1% host=%2% port=%3% password=%4% dbname=%5%")
        % _user % _host % _port % _password % _name;
    
    return connection_info.str();
};

// DBConnection
size_t DBConnection::connect(const DBConnectionOption& connection_option){
    try{
        connection_ = std::shared_ptr<pqxx::connection>(new pqxx::connection(connection_option.getConnectionInfo()));
        option_ = connection_option;
        return EXIT_SUCCESS;
    }
    catch(const std::exception& e){
        std::cerr << "Connection is failed!" << std::endl;
        return EXIT_FAILURE;
    }
};

size_t DBConnection::disconnect(){
    try{
        connection_->disconnect();
        return EXIT_SUCCESS;
    }
    catch(const std::exception& e){
        std::cerr << "Connection is already closed!" << std::endl;
        return EXIT_FAILURE;
    }
};

std::shared_ptr<pqxx::connection> DBConnection::getConnection(){return connection_;};

DBConnectionOption DBConnection::getOption() const{return option_;};

pqxx::result DBConnection::doRequest(const std::string& request){
    pqxx::result result;
    try{
        pqxx::work transaction{*connection_};
        result = transaction.exec(request);
        transaction.commit();
    }
    catch(const std::exception& e){
        std::cerr << "Request cannot be executed!" << std::endl;
        std::cerr << e.what() << std::endl;
    }
    
    return result;
};