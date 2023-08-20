#include "connection.h"
#include <boost/format.hpp>
#include <memory>
#include <iostream>

// DBConnectionOption
DBConnectionOption::DBConnectionOption() :
    db_host(DEFAULT_DBHOST), db_port(DEFAULT_DBPORT), db_user(DEFAULT_DBUSER), db_password(DEFAULT_DBPASSWORD), db_name(DEFAULT_DBNAME){};

DBConnectionOption::DBConnectionOption(const std::string& host, const std::string& port, const std::string& user, const std::string& password, const std::string& name) :
        db_host(host), db_port(port), db_user(user), db_password(password), db_name(name){};

std::string DBConnectionOption::getConnectionInfo() const{
    auto connection_info = boost::format("user=%1% host=%2% port=%3% password=%4% dbname=%5%")
        % db_user % db_host % db_port % db_password % db_name;
    
    return connection_info.str();
};

// DBConnection
DBConnection::~DBConnection(){
    if(this->isActive())
        this->disconnect();
};

size_t DBConnection::connect(const DBConnectionOption& db_info){
    try{
        connection_ = std::shared_ptr<pqxx::connection>(new pqxx::connection(db_info.getConnectionInfo()));
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};

size_t DBConnection::disconnect(){
    try{
        connection_->disconnect();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};

bool DBConnection::isActive() const{
    return (connection_) ? (connection_->is_open()) : (false);
};

std::shared_ptr<pqxx::connection> DBConnection::getConnection() const{
    return connection_;
};

pqxx::result DBConnection::doRequest(std::shared_ptr<BaseRequestBody> request){
    pqxx::result result;

    try{
        pqxx::work transaction{*connection_};
        result = transaction.exec(request->createRequest());
        transaction.commit();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    
    return result;
};