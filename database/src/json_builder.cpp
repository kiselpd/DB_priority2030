#include "json_builder.h"
#include "json.hpp"
#include <vector>
#include <boost/algorithm/string/join.hpp>

// BuilderSelectRequest
BuilderSelectRequest::BuilderSelectRequest(const std::string& request) : str_request_(request){};

std::shared_ptr<DBBaseRequest> BuilderSelectRequest::build(){
    nlohmann::json json_request = nlohmann::json::parse(str_request_);
    std::shared_ptr<DBSelectRequest> db_request = std::make_shared<DBSelectRequest>();
    if(json_request.contains("target"))
        db_request->_target = json_request["target"];

    if(json_request.contains("source"))
        db_request->_source = json_request["source"];

    if(json_request.contains("option"))
        db_request->_option = json_request["option"];

    if(json_request.contains("limit"))
        db_request->_limit = (size_t)json_request["limit"];

    return db_request;
};

// BuilderInsertRequest
BuilderInsertRequest::BuilderInsertRequest(const std::string& request) : str_request_(request){};

std::shared_ptr<DBBaseRequest> BuilderInsertRequest::build(){
    nlohmann::json json_request = nlohmann::json::parse(str_request_);
    std::shared_ptr<DBInsertRequest> db_request = std::make_shared<DBInsertRequest>();

    if(json_request.contains("source"))
        db_request->_source = json_request["source"];

    if(json_request.contains("target")){
        std::vector<std::string> target;
        for (auto& elem : json_request["target"])
            target.push_back(elem);

        db_request->_target = boost::algorithm::join(target, ",");
    }

    if(json_request.contains("value")){
        std::vector<std::string> all_value_vector;
        for (auto& array : json_request["value"]){
            std::vector<std::string> value_vector;
            for(auto& elem : array)
                value_vector.push_back("\'" + (std::string)elem + "\'");
            std::string value = "(" + boost::algorithm::join(value_vector, ",") + ")";
            all_value_vector.push_back(value);
        }

        db_request->_value = boost::algorithm::join(all_value_vector, ",");
    }

    return db_request;
};
// BuilderUpdateRequest
BuilderUpdateRequest::BuilderUpdateRequest(const std::string& request) : str_request_(request){};

std::shared_ptr<DBBaseRequest> BuilderUpdateRequest::build(){
    nlohmann::json json_request = nlohmann::json::parse(str_request_);
    std::shared_ptr<DBUpdateRequest> db_request = std::make_shared<DBUpdateRequest>();

    if(json_request.contains("source"))
        db_request->_source = json_request["source"];

    if(json_request.contains("target") && json_request.contains("value")){
        std::vector<std::string> target_vector;
        auto target = json_request["target"];
        auto value = json_request["value"];
        for (size_t i = 0; i < target.size(); i++)
            target_vector.push_back((std::string)target[i] + "=" + (std::string)value[i]);
        
        db_request->_target = boost::algorithm::join(target_vector, ",");
    }

    if(json_request.contains("option"))
        db_request->_option = json_request["option"];

    return db_request;
};

// JsonDirector
std::shared_ptr<DBBaseRequest> JsonDirector::getRequest(){
    std::shared_ptr<DBBaseRequest> request;
    if(builder_)
        request = builder_->build();
    return request;        
};

void JsonDirector::setBuilder(std::shared_ptr<BaseBuilder> builder){
    builder_ = builder;
};

// BuilderAnswer
BuilderAnswer::BuilderAnswer(pqxx::result answer) : answer_(answer){};

std::shared_ptr<DBBaseRequest> BuilderAnswer::build(){
    nlohmann::json json_answer;
    for(auto row : answer_){
        for(auto item : row){
            std::cout << item.c_str() <<  " ";
        }
    }
};
