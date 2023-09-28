#ifndef API_REQUEST_H
#define API_REQUEST_H

#include <iostream>

namespace request
{
    namespace header
    {
        const std::string header = "header";
        const std::string method = "method";
        const std::string token = "token";
        const std::string date_stamp = "datestamp";
        const std::string time_stamp = "timestamp";
    };

    namespace body
    {
        const std::string body = "body";
    };
};

class RequestHeader
{
public:
    bool fill(const std::string &request_header);

    std::string _method;
    std::string _token;
    std::string _date_stamp;
    std::string _time_stamp;
};

class RequestBody
{
public:
    bool fill(const std::string &request_body);

    std::string _json_request;
};

class RequestToAPI
{
public:
    bool fill(const std::string &request);

    std::string getToken() const;
    std::string getMethod() const;
    std::string getRequest() const;

private:
    std::string getHeaderFromRequest_(const std::string &request);
    std::string getBodyFromRequest_(const std::string &request);

    RequestHeader header_;
    RequestBody body_;
};

#endif /*API_REQUEST_H*/