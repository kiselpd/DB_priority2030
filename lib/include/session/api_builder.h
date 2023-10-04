#ifndef API_BUILDER_H
#define API_BUILDER_H

#include <memory>

#include "request.h"

class BaseAPIBuilder
{
public:
    virtual std::shared_ptr<DBBaseRequest> build() = 0;
};

class SelectAPIBuilder : public BaseAPIBuilder
{
public:
    SelectAPIBuilder(const std::string &id, const std::string &request);
    std::shared_ptr<DBBaseRequest> build() final;

private:
    std::string id_;
    std::string str_request_;
};

class InsertAPIBuilder : public BaseAPIBuilder
{
public:
    InsertAPIBuilder(const std::string &id, const std::string &request);
    std::shared_ptr<DBBaseRequest> build() final;

private:
    std::string id_;
    std::string str_request_;
};

class UpdateAPIBuilder : public BaseAPIBuilder
{
public:
    UpdateAPIBuilder(const std::string &id, const std::string &request);
    std::shared_ptr<DBBaseRequest> build() final;

private:
    std::string id_;
    std::string str_request_;
};

class DeleteAPIBuilder : public BaseAPIBuilder
{
public:
    DeleteAPIBuilder(const std::string &id, const std::string &request);
    std::shared_ptr<DBBaseRequest> build() final;

private:
    std::string id_;
    std::string str_request_;
};

class AuthAPIBuilder : public BaseAPIBuilder
{
public:
    AuthAPIBuilder(const std::string &request);
    std::shared_ptr<DBBaseRequest> build() final;

private:
    std::string str_request_;
};

#endif /*API_BUILDER_H*/