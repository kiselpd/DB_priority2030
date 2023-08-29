#ifndef REQUEST_H
#define REQUEST_H

#include<iostream>

class DBBaseRequest{
public:
    virtual std::string createRequest() const = 0;
};


class DBSelectRequest : public DBBaseRequest
{
public:
    std::string _source;
    std::string _target;
    std::string _option;
    size_t _limit = 0;

    std::string createRequest() const override;
};


class DBInsertRequest : public DBBaseRequest
{
public:
    std::string _source;
    std::string _target;
    std::string _value;

    std::string createRequest() const override;
};


class DBUpdateRequest : public DBBaseRequest
{
public:
    std::string _source;
    std::string _target;
    std::string _option;

    std::string createRequest() const override;
};


class DBDeleteRequest : public DBBaseRequest
{
public:
    std::string _source;
    std::string _option;

    std::string createRequest() const override;
};

#endif /*REQUEST_H*/