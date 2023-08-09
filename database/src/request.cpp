#include "request.h"
#include <boost/format.hpp>

// BaseSelectRequest
BaseSelectRequest::BaseSelectRequest(const long int& id) : id_(id) {};

std::string BaseSelectRequest::createRequest() const{
    return this->createSelectRequest();
};

// AuthorizationSelectRequest
AuthorizationSelectRequest::AuthorizationSelectRequest(const std::string& username, const std::string& password) : 
    username_(username), password_(password) {};

std::string AuthorizationSelectRequest::createSelectRequest() const{
    auto request = boost::format(
        "SELECT * "
        "FROM user_data "
        "WHERE (user_login='%1%' OR user_email='%1%') AND user_password='%2%'") % this->username_ % this->password_;

    return request.str();
};