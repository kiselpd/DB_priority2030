#ifndef CLIENT_PARSER_H
#define CLIENT_PARSER_H

#include <iostream>
#include <memory>
#include "json.hpp"
#include "pqxx/pqxx"

namespace request_parser
{
    enum error_type
    {
        SUCCESS,
        JSON_ERROR,
        TITLE_ERROR,
        BODY_ERROR,
        METHOD_ERROR,
        SOURCE_ERROR,
        TARGET_ERROR,
        OPTION_ERROR
    };

    class error
    {
    public:
        error();
        error(const error_type& error);

        void setError(const error_type& error);
        error_type getError() const;
        std::string returnDescription() const;

        bool operator!() const;

    private:
        error_type error_;
    };

    namespace request_method
    {
        const std::string GET = "GET";
        const std::string POST = "POST";
        const std::string PUT = "PUT";
        const std::string DELETE = "DELETE";
    };
};


class RequestTitle
{
public:
    RequestTitle(const std::string& method, const std::string& id, const std::string& role, const std::string& time);
    void set(const std::string& method, const std::string& id, const std::string& role, const std::string& time);

    std::string method() const;
    std::string id() const;
    std::string role() const;
    std::string time() const;

private:
    std::string method_;
    std::string id_;
    std::string role_;
    std::string time_;
};

namespace answer_parser
{
    enum status
    {
        OK = 200,
        Bad_Request = 400
    };
};


class AnswerTitle
{
public:
    AnswerTitle(const answer_parser::status& status_code, const std::string& time);
    void set(const answer_parser::status& status_code, const std::string& time);

    size_t status_code() const;
    std::string time() const;
private:

    answer_parser::status status_code_;
    std::string time_;
};


class AnswerBody
{
public:
    AnswerBody(const pqxx::result& result);
    void set(const pqxx::result& result);

    size_t count() const;
    pqxx::result result() const;
    std::string errorDescription() const { return "error";};//!!!!

    bool isSuccess() const{return true;};//!!!!

private:
    pqxx::result result_;
};


class BaseRequestBody
{
public:
    BaseRequestBody(const std::string& source, const std::string& target);
    virtual std::string createRequest() = 0;

    std::string source_;
    std::string target_;
};


class SelectRequestBody : public BaseRequestBody
{
public:
    SelectRequestBody(const std::string& source, const std::string& target, const std::string& option, const std::string& limit);
    std::string createRequest() override;

private:
    std::string option_;
    std::string limit_;
};


class UpdateRequestBody : public BaseRequestBody
{

};


class InsertRequestBody : public BaseRequestBody
{

};


class ClientParser
{
public:
    std::pair<std::shared_ptr<RequestTitle>, std::shared_ptr<BaseRequestBody>> parseFromJson(const std::string& json_request, request_parser::error& error);
    std::string parseToJson(const AnswerTitle& answer_title, const AnswerBody& answer_body);

private:
    std::shared_ptr<RequestTitle> parseTitleFromJson_(const nlohmann::json& title, request_parser::error& error);
    std::shared_ptr<BaseRequestBody> parseBodyFromJson_(const std::string& method, const nlohmann::json& body, request_parser::error& error);

    std::shared_ptr<SelectRequestBody> parseGetBodyFromJson_(const nlohmann::json& body, request_parser::error& error);
    // std::shared_ptr<SelectRequestBody> parsePostBodyFromJson_(const nlohmann::json& body, request_parser::error& error);
    // std::shared_ptr<SelectRequestBody> parsePutBodyFromJson_(const nlohmann::json& body, request_parser::error& error);
    // std::shared_ptr<SelectRequestBody> parseDeleteBodyFromJson_(const nlohmann::json& body, request_parser::error& error);

    nlohmann::json parseTitleToJson_(const AnswerTitle& answer_title);
    nlohmann::json parseBodyToJson_(const AnswerBody& answer_body);
};

#endif /*CLIENT_PARSER_H*/