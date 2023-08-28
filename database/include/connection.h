#ifndef CONNECTION_H
#define CONNECTION_H

#include <pqxx/pqxx> 
#include "client_parser.h"

struct DBConnectionOption
{
    DBConnectionOption(const std::string& host, const std::string& port,
        const std::string& user, const std::string& password, const std::string& name);

    DBConnectionOption(){};

    std::string getConnectionInfo() const;

    std::string _host;
    std::string _port;
    std::string _user;
    std::string _password;
    std::string _name;
};


class DBConnection
{
public:
    size_t connect(const DBConnectionOption& connection_option);
    size_t disconnect();

    std::shared_ptr<pqxx::connection> getConnection();
    DBConnectionOption getOption() const;
    pqxx::result doRequest(const std::string& request);
    
private:
    std::shared_ptr<pqxx::connection> connection_;
    DBConnectionOption option_;
};

#endif /*CONNECTION_H*/