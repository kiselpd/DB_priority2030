#include "api_request.h"

// RequestHeader
bool RequestHeader::fill(const nlohmann::json &request_header)
{
    try
    {
        _method = request_header[request::header::method];

        if (request_header.contains(request::header::token))
            _token = request_header[request::header::token];
    }
    catch (const std::exception &e)
    {
        std::cerr << "Request header parse error!" << '\n';
        std::cerr << e.what() << '\n';
    }

    return !_method.empty();
};

// RequestBody
bool RequestBody::fill(const nlohmann::json &request_body)
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
    std::cout << request << std::endl;
    return header_.fill(this->getHeaderFromRequest_(request)) && body_.fill(this->getBodyFromRequest_(request));
};

nlohmann::json RequestToAPI::getHeaderFromRequest_(const std::string &request)
{
    nlohmann::json header;
    try
    {
        auto json_request = nlohmann::json::parse(request);
        header = json_request[request::header::header];
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "No header in request!" << '\n';
    }
    return header;
};

nlohmann::json RequestToAPI::getBodyFromRequest_(const std::string &request)
{
    nlohmann::json body;
    try
    {
        auto json_request = nlohmann::json::parse(request);
        body = json_request[request::body::body];
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
    return body_._json_request.dump();
};