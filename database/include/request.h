#ifndef REQUEST_H
#define REQUEST_H

#include<iostream>

class RequestTitle
{
public:
    RequestTitle(const size_t& id, const std::string& method, const std::string& time);
    void set(const size_t& id, const std::string& method, const std::string& time);

    std::string method() const;
    size_t id() const;
    std::string role() const;
    std::string time() const;

private:
    std::string method_;
    size_t id_;
    std::string role_;
    std::string time_;
};

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

#endif /*REQUEST_H*/