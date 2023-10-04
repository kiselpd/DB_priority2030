#include "api_builder.h"

#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

// SelectAPIBuilder
SelectAPIBuilder::SelectAPIBuilder(const std::string &id, const std::string &request) : id_(id),
                                                                                        str_request_(request){};

std::shared_ptr<DBBaseRequest> SelectAPIBuilder::build()
{
    nlohmann::json json_request = nlohmann::json::parse(str_request_);
    std::shared_ptr<DBSelectRequest> db_request = std::make_shared<DBSelectRequest>();

    if (json_request.contains("target"))
        db_request->_target = json_request["target"];

    if (json_request.contains("source"))
        db_request->_source = json_request["source"];

    db_request->_option = "user_id = '" + id_ + "'";

    if (json_request.contains("option"))
        db_request->_option += " AND " + (std::string)json_request["option"];

    if (json_request.contains("limit"))
        db_request->_limit = (size_t)json_request["limit"];

    return db_request;
};

// InsertAPIBuilder
InsertAPIBuilder::InsertAPIBuilder(const std::string &id, const std::string &request) : id_(id),
                                                                                        str_request_(request){};

std::shared_ptr<DBBaseRequest> InsertAPIBuilder::build()
{
    nlohmann::json json_request = nlohmann::json::parse(str_request_);
    std::shared_ptr<DBInsertRequest> db_request = std::make_shared<DBInsertRequest>();

    if (json_request.contains("source"))
        db_request->_source = json_request["source"];

    if (json_request.contains("target"))
    {
        std::vector<std::string> target;

        for (auto &elem : json_request["target"])
            target.push_back(elem);

        db_request->_target = boost::algorithm::join(target, ",");
    }

    if (json_request.contains("value"))
    {
        std::vector<std::string> all_value_vector;

        for (auto &array : json_request["value"])
        {
            std::vector<std::string> value_vector;

            for (auto &elem : array)
                value_vector.push_back("\'" + (std::string)elem + "\'");

            std::string value = "(" + boost::algorithm::join(value_vector, ",") + ")";
            all_value_vector.push_back(value);
        }

        db_request->_value = boost::algorithm::join(all_value_vector, ",");
    }

    return db_request;
};

// UpdateAPIBuilder
UpdateAPIBuilder::UpdateAPIBuilder(const std::string &id, const std::string &request) : id_(id),
                                                                                        str_request_(request){};

std::shared_ptr<DBBaseRequest> UpdateAPIBuilder::build()
{
    nlohmann::json json_request = nlohmann::json::parse(str_request_);
    std::shared_ptr<DBUpdateRequest> db_request = std::make_shared<DBUpdateRequest>();

    if (json_request.contains("source"))
        db_request->_source = json_request["source"];

    if (json_request.contains("target") && json_request.contains("value"))
    {
        std::vector<std::string> target_vector;
        auto target = json_request["target"];
        auto value = json_request["value"];

        for (size_t i = 0; i < target.size(); i++)
            target_vector.push_back((std::string)target[i] + "=" + (std::string)value[i]);

        db_request->_target = boost::algorithm::join(target_vector, ",");
    }

    db_request->_option = "user_id = '" + id_ + "'";

    if (json_request.contains("option"))
        db_request->_option += " AND " + (std::string)json_request["option"];

    return db_request;
};

// DeleteAPIBuilder
DeleteAPIBuilder::DeleteAPIBuilder(const std::string &id, const std::string &request) : id_(id),
                                                                                        str_request_(request){};

std::shared_ptr<DBBaseRequest> DeleteAPIBuilder::build()
{
    nlohmann::json json_request = nlohmann::json::parse(str_request_);
    std::shared_ptr<DBDeleteRequest> db_request = std::make_shared<DBDeleteRequest>();

    if (json_request.contains("source"))
        db_request->_source = json_request["source"];

    db_request->_option = "user_id = '" + id_ + "'";

    if (json_request.contains("option"))
        db_request->_option += " AND " + (std::string)json_request["option"];

    return db_request;
};

// AuthAPIBuilder
AuthAPIBuilder::AuthAPIBuilder(const std::string &request) : str_request_(request){};

std::shared_ptr<DBBaseRequest> AuthAPIBuilder::build()
{
    nlohmann::json json_request = nlohmann::json::parse(str_request_);

    if (!(json_request.contains("username") && json_request.contains("password")))
        return nullptr;

    std::shared_ptr<DBSelectRequest> db_request = std::make_shared<DBSelectRequest>();

    db_request->_source = "user_info";

    auto option_format = boost::format("(login='%1%' OR email='%1%') AND password='%2%'") % (std::string)json_request["username"] % (std::string)json_request["password"];
    db_request->_option = option_format.str();

    return db_request;
};
