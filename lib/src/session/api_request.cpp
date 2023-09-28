#include "api_request.h"

#include "nlohmann/json.hpp"

// RequestHeader
bool RequestHeader::fill(const std::string &request_header)
{
    try
    {
        nlohmann::json json_header = nlohmann::json::parse(request_header);
        _method = json_header[request::header::method];

        if (json_header.contains(request::header::token))
            _token = json_header[request::header::token];
    }
    catch (const std::exception &e)
    {
        std::cerr << "Request header parse error!" << '\n';
        std::cerr << e.what() << '\n';
    }

    return !_method.empty();
};

// RequestBody
bool RequestBody::fill(const std::string &request_body)
{
    if (!request_body.empty())
        this->_json_request = request_body;
    else
        std::cerr << "Request body is empty!" << '\n';

    return !_json_request.empty();
};

// RequestToAPI
bool RequestToAPI::fill(const std::string &request)
{
    return header_.fill(this->getHeaderFromRequest_(request)) && body_.fill(this->getBodyFromRequest_(request));
};

std::string RequestToAPI::getHeaderFromRequest_(const std::string &request)
{
    std::string header;
    try
    {
        nlohmann::json json_request = nlohmann::json::parse(request);
        header = json_request[request::header::header].dump();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "No header in request!" << '\n';
    }
    return header;
};

std::string RequestToAPI::getBodyFromRequest_(const std::string &request)
{
    std::string body;
    try
    {
        nlohmann::json json_request = nlohmann::json::parse(request);
        body = json_request[request::body::body].dump();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "No body in request!" << '\n';
    }
    return body;
};

std::string RequestToAPI::getToken() const
{
    return header_._token;
};

std::string RequestToAPI::getMethod() const
{
    return header_._method;
};

std::string RequestToAPI::getRequest() const
{
    return body_._json_request;
};