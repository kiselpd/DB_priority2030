#ifndef JSON_BUILDER_H
#define JSON_BUILDER_H

#include<iostream>
#include "request.h"
#include <memory>

class BaseBuilder
{
public:
    virtual std::shared_ptr<DBBaseRequest> build() = 0;
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


class BuilderDeleteRequest : public BaseBuilder
{
public:
    BuilderDeleteRequest(const std::string& request);
    std::shared_ptr<DBBaseRequest> build() override;

private:
    std::string str_request_;
};


std::string getBody(const std::string& request);
std::string getType(const std::string& request);

#endif /*JSON_BUILDER_H*/