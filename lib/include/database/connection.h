#ifndef CONNECTION_H
#define CONNECTION_H

#include <pqxx/pqxx>

class DBConnectionOption
{
public:
    size_t setConnectionOptionFromFile(const std::string &file_name);
    std::string getConnectionInfo() const;

    std::string _host;
    std::string _port;
    std::string _user;
    std::string _password;
    std::string _dbname;

private:
    size_t getConnectionOptionFromJson_(std::shared_ptr<std::ifstream> ifs);
};

class DBConnection
{
public:
    size_t connect(const DBConnectionOption &connection_option);
    size_t disconnect();

    std::shared_ptr<pqxx::connection> getConnection();
    pqxx::result doRequest(const std::string &request);

private:
    std::shared_ptr<pqxx::connection> connection_;
};

#endif /*CONNECTION_H*/