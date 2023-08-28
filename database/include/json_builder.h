#ifndef JSON_BUILDER_H
#define JSON_BUILDER_H

#include<iostream>
#include "request.h"
#include <memory>
#include<pqxx/pqxx>

class BaseBuilder
{
public:
    virtual std::shared_ptr<DBBaseRequest> build() = 0;
};

class BuilderAnswer : public BaseBuilder
{
public:
    BuilderAnswer(pqxx::result answer);
    std::shared_ptr<DBBaseRequest> build() override;

private:
    pqxx::result answer_;
};

class BuilderSelectRequest : public BaseBuilder
{
public:
    BuilderSelectRequest(const std::string& request);
    std::shared_ptr<DBBaseRequest> build() override;

private:
    std::string str_request_;
};


class BuilderInsertRequest : public BaseBuilder
{
public:
    BuilderInsertRequest(const std::string& request);
    std::shared_ptr<DBBaseRequest> build() override;

private:
    std::string str_request_;
};


class BuilderUpdateRequest : public BaseBuilder
{
public:
    BuilderUpdateRequest(const std::string& request);
    std::shared_ptr<DBBaseRequest> build() override;

private:
    std::string str_request_;
};


// class BuilderDeleteRequest : public BaseBuilder
// {
// public:
//     BuilderDeleteRequest(const size_t& id, const std::string& request);
//     std::shared_ptr<DBBaseRequest> buildRequest() override;

// private:
//     size_t id_;
//     std::string str_request_;
// };

class JsonDirector
{
public:
    std::shared_ptr<DBBaseRequest> getRequest();
    void setBuilder(std::shared_ptr<BaseBuilder> builder);

private:
    std::shared_ptr<BaseBuilder> builder_;
};

#endif /*JSON_BUILDER_H*/