#include "api_answer.h"

#include <nlohmann/json.hpp>

// ErrorAPIAnswer
ErrorAPIAnswer::ErrorAPIAnswer(const StatusAPICode &error, const std::string &description)
{
    nlohmann::json json_answer;
    json_answer["header"] = addHeader_(error);
    json_answer["body"] = addBody_(description);
    answer_ = json_answer.dump();
};

std::string ErrorAPIAnswer::get() const
{
    return answer_;
};

nlohmann::json ErrorAPIAnswer::addHeader_(const StatusAPICode &error) const
{
    nlohmann::json json_header;
    json_header["status"] = (int)error;
    return json_header;
};

nlohmann::json ErrorAPIAnswer::addBody_(const std::string &description) const
{
    nlohmann::json json_body;
    json_body["error"] = description;
    return json_body;
};

// ResultAPIAnswer
ResultAPIAnswer::ResultAPIAnswer(const StatusAPICode &status, const pqxx::result &result)
{
    nlohmann::json json_answer;
    json_answer["header"] = addHeader_(status);
    json_answer["body"] = addBody_(result);
    answer_ = json_answer.dump();
};

ResultAPIAnswer::ResultAPIAnswer(const StatusAPICode &status, const std::string &jwt_token)
{
    nlohmann::json json_answer;
    json_answer["header"] = addHeader_(status);
    json_answer["body"] = addBody_(jwt_token);
    answer_ = json_answer.dump();
};

nlohmann::json ResultAPIAnswer::addHeader_(const StatusAPICode &status) const
{
    nlohmann::json json_header;
    json_header["status"] = (int)status;
    return json_header;
};

nlohmann::json ResultAPIAnswer::addBody_(const pqxx::result &result) const
{
    nlohmann::json json_body;
    json_body["count"] = result.size();
    nlohmann::json all_value = nlohmann::json::array();

    for (const auto &row : result)
    {
        nlohmann::json value = nlohmann::json::array();
        for (const auto &field : row)
        {
            value.push_back(field.c_str());
        }
        all_value.push_back(value);
    }
    json_body["value"] = all_value;

    return json_body;
};

nlohmann::json ResultAPIAnswer::addBody_(const std::string &jwt_token) const
{
    nlohmann::json json_body;
    json_body["value"] = jwt_token;
    return json_body;
};

std::string ResultAPIAnswer::get() const
{
    return answer_;
};