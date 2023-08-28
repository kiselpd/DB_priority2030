#include "answer.h"

// Answer
Answer::Answer(pqxx::result result) : result_(result){};

std::string Answer::getAnswer() const{
    nlohmann::json json_answer;
    json_answer["header"] = this->getHeader();
    json_answer["body"] = this->getBody();
    return json_answer.dump();
};

nlohmann::json Answer::getHeader() const{
    nlohmann::json json_header;
    json_header = (result_.size()) ? (200) : (404);
    return json_header;
};

nlohmann::json Answer::getBody() const{
    nlohmann::json json_body;
    json_body["count"] = result_.size();
    nlohmann::json all_value = nlohmann::json::array();

    for(const auto& row : result_){
        nlohmann::json value = nlohmann::json::array();
        for(const auto& field : row){
            value.push_back(field.c_str());
        }
        all_value.push_back(value);
    }
    json_body["value"] = all_value;

    return json_body;
};

