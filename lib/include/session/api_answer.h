#ifndef API_ANSWER_H
#define API_ANSWER_H

#include <iostream>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

enum StatusAPICode
{
    OK = 200,
    BadRequest = 400,
    Unauthorized = 401,
    NotFound = 404
};

class BaseAPIAnswer
{
public:
    virtual std::string get() const = 0;
};

class ErrorAPIAnswer : public BaseAPIAnswer
{
public:
    ErrorAPIAnswer(const StatusAPICode &error, const std::string &description);
    std::string get() const final;

private:
    nlohmann::json addHeader_(const StatusAPICode &error) const;
    nlohmann::json addBody_(const std::string &description) const;

    std::string answer_;
};

class ResultAPIAnswer : public BaseAPIAnswer
{
public:
    ResultAPIAnswer(const StatusAPICode &status, const pqxx::result &result);
    ResultAPIAnswer(const StatusAPICode &status, const std::string &jwt_token);

    nlohmann::json addHeader_(const StatusAPICode &status) const;
    nlohmann::json addBody_(const pqxx::result &result) const;
    nlohmann::json addBody_(const std::string &jwt_token) const;

    std::string get() const final;

private:
    std::string answer_;
};

#endif /*API_ANSWER_H*/