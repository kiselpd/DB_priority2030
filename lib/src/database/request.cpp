#include "request.h"
#include <boost/format.hpp>

// DBSelectRequest
std::string DBSelectRequest::createRequest() const
{
    std::string str_request;
    if (!_source.empty())
    {
        auto request = boost::format(
                           "SELECT %1% "
                           "FROM %2%") % (!_target.empty() ? _target : "*") % _source;

        str_request = request.str();
        if (!_option.empty())
            str_request += " WHERE " + _option;

        if (_limit)
            str_request += " LIMIT " + std::to_string(_limit);

        str_request += ";";
    }
    return str_request;
};

// DBInsertRequest
std::string DBInsertRequest::createRequest() const
{
    std::string str_request;
    if (!_source.empty() && !_target.empty() && !_value.empty())
    {
        auto request = boost::format(
                           "INSERT INTO %1%(%2%) "
                           "VALUES %3% "
                           "RETURNING *") % _source % _target % _value;

        str_request = request.str();
    }
    return str_request;
};

// DBUpdateRequest
std::string DBUpdateRequest::createRequest() const
{
    std::string str_request;
    if (!_source.empty() && !_target.empty())
    {
        auto request = boost::format(
                           "UPDATE %1% "
                           "SET %2%") % _source % _target;

        str_request = request.str();
        if (!_option.empty())
            str_request += " WHERE " + _option;

        str_request += " RETURNING * ;";
    }
    return str_request;
};

// DBDeleteRequest
std::string DBDeleteRequest::createRequest() const
{
    std::string str_request;
    if (!_source.empty())
    {
        str_request += "DELETE FROM " + _source;
        if (!_option.empty())
            str_request += " WHERE " + _option;

        str_request += " RETURNING * ;";
    }
    return str_request;
};