#include "client_parser.h"
#include "json.hpp"
#include <boost/format.hpp>

namespace request_parser
{
    error::error() : error_(error_type::SUCCESS) {};

    error::error(const error_type& error) : error_(error) {};

    void error::setError(const error_type& error){
        error_ = error;
    };

    error_type error::getError() const{
        return error_;
    };

    std::string error::returnDescription() const{
        return "ERROR REQUEST";
    };

    bool error::operator!() const{
        return error_ == error_type::SUCCESS;
    };
};

// RequestTitle
RequestTitle::RequestTitle(const std::string& method, const std::string& id, const std::string& role, const std::string& time) :
    method_(method), id_(id), role_(role), time_(time) {};

void RequestTitle::set(const std::string& method, const std::string& id, const std::string& role, const std::string& time){
    method_ = method;
    id_ = id;
    role_ = role;
    time_ = time;
};

std::string RequestTitle::method() const{return method_;};

std::string RequestTitle::id() const{return id_;};

std::string RequestTitle::role() const{return role_;};

std::string RequestTitle::time() const{return time_;};

// AnswerTitle
AnswerTitle::AnswerTitle(const answer_parser::status& status_code, const std::string& time) :
    status_code_(status_code), time_(time) {};

void AnswerTitle::set(const answer_parser::status& status_code, const std::string& time){
    status_code_ = status_code;
    time_ = time;
};

size_t AnswerTitle::status_code() const{
    return status_code_;
};

std::string AnswerTitle::time() const{
    return time_;
};

// AnswerBody
AnswerBody::AnswerBody(const pqxx::result& result) : result_(result) {};

void AnswerBody::set(const pqxx::result& result){
    result_ = result;
};

size_t AnswerBody::count() const{
    return result_.size();
};

pqxx::result AnswerBody::result() const{
    return result_;
};

//BaseRequestBody
BaseRequestBody::BaseRequestBody(const std::string& source, const std::string& target) :
    source_(source), target_(target) {};

//SelectRequestBody
SelectRequestBody::SelectRequestBody(const std::string& source, const std::string& target, const std::string& option, const std::string& limit) :
    BaseRequestBody(source, target), option_(option), limit_(limit) {};

std::string SelectRequestBody::createRequest(){
    auto request = boost::format(
        "SELECT %1% "
        "FROM %2% "
        "WHERE %3%") % ((target_.empty()) ? ("*") : (target_)) % source_ % option_;
    
    std::string str_request = request.str();

    if(!limit_.empty()){
        auto request_limit = boost::format(" LIMIT %1%") % limit_;
        str_request += request_limit.str();
    }

    return str_request + ";";
};

//ClientParser
std::pair<std::shared_ptr<RequestTitle>, std::shared_ptr<BaseRequestBody>> ClientParser::parseFromJson(const std::string& json_request, request_parser::error& error){
    try{
        nlohmann::json json = nlohmann::json::parse(json_request);
        auto request_title = this->parseTitleFromJson_(json["title"], error);
        auto request_body = this->parseBodyFromJson_(request_title->method(), json["body"], error);
        return {request_title, request_body};
    }
    catch(const std::exception& e){
        error.setError(request_parser::error_type::JSON_ERROR);
        return {nullptr, nullptr};
    }
};

std::shared_ptr<RequestTitle> ClientParser::parseTitleFromJson_(const nlohmann::json& title, request_parser::error& error){
    try
    {
        auto method = (std::string)title["method"];
        auto id = (std::string)title["id"];
        auto role = (std::string)title["role"];
        auto time = (std::string)title["time"];

        std::shared_ptr<RequestTitle> request_title = std::make_shared<RequestTitle>(method, id, role, time);
        return request_title;
    }
    catch(const std::exception& e)
    {
        error.setError(request_parser::error_type::TITLE_ERROR);
        return nullptr;
    }
    
};

std::shared_ptr<BaseRequestBody> ClientParser::parseBodyFromJson_(const std::string& method, const nlohmann::json& body, request_parser::error& error){
    if(method == request_parser::request_method::GET)
        return this->parseGetBodyFromJson_(body, error);
    // if(method == request_parser::request_method::PUT)
    //     return this->parsePutBodyFromJson_(body, error);
    // if(method == request_parser::request_method::POST)
    //     return this->parsePostBodyFromJson_(body, error);
    // if(method == request_parser::request_method::DELETE)
    //     return this->parseDeleteBodyFromJson_(body, error);

    error.setError(request_parser::error_type::METHOD_ERROR);
    return nullptr;
};

std::shared_ptr<SelectRequestBody> ClientParser::parseGetBodyFromJson_(const nlohmann::json& body, request_parser::error& error){
    std::string source, target, option, limit;
    if(auto it = body.find("source"); it != body.end())
        source = (std::string)(it.value());
    else{
        error.setError(request_parser::error_type::SOURCE_ERROR);
        return nullptr;
    }

    if(auto it = body.find("target"); it != body.end())
        target = (std::string)(it.value());

    if(auto it = body.find("option"); it != body.end())
        option = (std::string)(it.value());
    else{
        error.setError(request_parser::error_type::OPTION_ERROR);
        return nullptr;
    }

    if(auto it = body.find("limit"); it != body.end())
        limit = (std::string)(it.value());

    return std::make_shared<SelectRequestBody>(source, target, option, limit);
};

std::string ClientParser::parseToJson(const AnswerTitle& answer_title, const AnswerBody& answer_body){
    nlohmann::json answer;
    answer["title"] = this->parseTitleToJson_(answer_title);
    answer["body"] = this->parseBodyToJson_(answer_body);
    return answer.dump();
};

nlohmann::json ClientParser::parseTitleToJson_(const AnswerTitle& answer_title){
    nlohmann::json title;
    title["status"] = answer_title.status_code();
    title["time"] = answer_title.time();
    return title;
};

nlohmann::json ClientParser::parseBodyToJson_(const AnswerBody& answer_body){
    nlohmann::json body;
    if(answer_body.isSuccess()){
        body["count"] = answer_body.count();
        pqxx::result result = answer_body.result();
        for(const auto& row: result){
            nlohmann::json array;
            for(const auto& field : row)
                array.emplace_back(field.c_str());
            
            body["result"].emplace_back(array);
        }
    }
    else
        body["error description"] = answer_body.errorDescription();

    return body;
};