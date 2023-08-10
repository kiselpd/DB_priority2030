#ifndef REQUEST
#define REQUEST

#include<iostream>

class BaseDBRequest{
public:
    virtual std::string createRequest() const = 0;
};


class BaseSelectRequest : public BaseDBRequest{
public:
    BaseSelectRequest(){};
    BaseSelectRequest(const long int& id);
    std::string createRequest() const override;
    virtual std::string createSelectRequest() const = 0;

private:
    long int id_;
};


class AuthorizationSelectRequest : public BaseSelectRequest{
public:
    AuthorizationSelectRequest(const std::string& username, const std::string& password);
    std::string createSelectRequest() const override;

private:
    std::string username_;
    std::string password_;
};


class ConsumersSelectRequest : public BaseSelectRequest{
public:
    ConsumersSelectRequest(const long int& user_id);
    std::string createSelectRequest() const override;
};


class EnergySelectRequest : public BaseSelectRequest{
public:
    EnergySelectRequest(const long int& user_id);
    std::string createSelectRequest() const override;
};

#endif /*REQUEST*/