#ifndef CONNECTION_H
#define CONNECTION_H

#include <pqxx/pqxx> 
#include "client_parser.h"

const std::string DEFAULT_DBHOST = "127.0.0.1";
const std::string DEFAULT_DBPORT = "5432";
const std::string DEFAULT_DBUSER = "postgres";
const std::string DEFAULT_DBPASSWORD = "qwerty";
const std::string DEFAULT_DBNAME = "Priority2030";


struct DBConnectionOption
{
    DBConnectionOption();
    DBConnectionOption(const std::string& host, const std::string& port,
        const std::string& user, const std::string& password, const std::string& name);

    std::string getConnectionInfo() const;

    std::string db_host;
    std::string db_port;
    std::string db_user;
    std::string db_password;
    std::string db_name;
};


class DBConnection
{
public:
    ~DBConnection();
    size_t connect(const DBConnectionOption& db_info);
    size_t disconnect();
    bool isActive() const;

    std::shared_ptr<pqxx::connection> getConnection() const;
    pqxx::result doRequest(std::shared_ptr<BaseRequestBody> request);
    
private:
    std::shared_ptr<pqxx::connection> connection_;
};

#endif /*CONNECTION_H*/