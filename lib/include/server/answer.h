#ifndef ANSWER_H
#define ANSWER_H

#include <iostream>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

class Answer
{
public:
    Answer(pqxx::result result);
    std::string getAnswer() const;

private:
    nlohmann::json getHeader() const;
    nlohmann::json getBody() const;

    pqxx::result result_;
};

#endif /*ANSWER_H*/