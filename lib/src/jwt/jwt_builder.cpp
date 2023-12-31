#include <jwt_builder.h>
#include <ctime>

// JWTPayload
JWTPayload::JWTPayload(const std::string &id, const std::string &role) : user_id_(id), user_role_(role){};

JWTPayload::JWTPayload(const jwt::jwt_payload &payload)
{
    if (payload.has_claim(user_id_field))
        this->user_id_ = payload.get_claim_value<std::string>(user_id_field);

    if (payload.has_claim(user_role_field))
        this->user_role_ = payload.get_claim_value<std::string>(user_role_field);
};

jwt::jwt_payload JWTPayload::get() const
{
    jwt::jwt_payload payload;
    payload.add_claim(user_id_field, user_id_);
    payload.add_claim(user_role_field, user_role_);
    return payload;
};

std::pair<std::string, std::string> JWTPayload::getInfo() const
{
    return {user_id_, user_role_};
};

// JWTBuilder
JWTBuilder::JWTBuilder(const std::vector<std::string> &list_algo, const std::string &secret) : list_algo_(list_algo), secret_(secret){};

jwt_token JWTBuilder::getToken(const JWTPayload &payload) const
{
    jwt_token token_str;
    try
    {
        jwt::jwt_header header;
        header.algo("HS256");

        jwt::jwt_object token;
        token.header(header);
        token.payload(payload.get());
        token.secret(secret_);
        token_str = token.signature();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Build token Error!" << '\n';
        std::cerr << e.what() << '\n';
    }
    return token_str;
};

JWTPayload JWTBuilder::getPayload(const jwt_token &token_str) const
{
    try
    {
        auto decode_token = jwt::decode(token_str, jwt::params::algorithms(list_algo_), jwt::params::secret(secret_));
        return JWTPayload(decode_token.payload());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Build token Error!" << '\n';
        std::cerr << e.what() << '\n';
        return JWTPayload();
    }
};

std::string JWTBuilder::getRandomAlgo_() const
{
    std::srand(std::time(nullptr));
    return list_algo_[(rand() % list_algo_.size())];
};