#ifndef JWT_BUILDER_H
#define JWT_BUILDER_H

#include <iostream>
#include <jwt/jwt.hpp>

const std::string user_id_field = "user_id";
const std::string user_role_field = "user_role";

typedef std::string jwt_token;

class JWTPayload
{
public:
    JWTPayload(){};
    JWTPayload(const std::string& id, const std::string& role);
    JWTPayload(const jwt::jwt_payload& payload);

    jwt::jwt_payload get() const;
    std::pair<std::string, std::string> getInfo() const;

private:
    std::string user_id_;
    std::string user_role_;
};

class JWTBuilder
{
public:
    JWTBuilder(const std::vector<std::string> &list_algo, const std::string &secret);
    jwt_token getToken(const JWTPayload &payload) const;
    JWTPayload getPayload(const jwt_token& token_str) const;

private:
    std::string getRandomAlgo_() const;

    std::vector<std::string> list_algo_;
    std::string secret_;
};

#endif /*JWT_BUILDER_H*/