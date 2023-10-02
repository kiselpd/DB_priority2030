#include "jwt_manager.h"

// JWTAuthenticationManager
JWTAuthenticationManager::JWTAuthenticationManager(std::shared_ptr<boost::asio::io_service> io_ptr, const std::vector<std::string> &list_algo, const std::string &secret) : collection_(std::make_shared<JWTCollection>()),
                                                                                                                                                                                 timer_(std::make_shared<JWTStatusTimer>(io_ptr, collection_)),
                                                                                                                                                                                 builder_(std::make_shared<JWTBuilder>(list_algo, secret)){};

bool JWTAuthenticationManager::isAuthenticated(const jwt_token &token)
{
    auto has_token = collection_->has(token);

    if (has_token)
    {
        auto [oldest_token, oldest_time] = collection_->getOldest();
        timer_->start(oldest_token, oldest_time);
    }

    return has_token;
};

jwt_token JWTAuthenticationManager::addNewJWT(const std::string &user_id, const std::string &user_role)
{
    JWTPayload payload(user_id, user_role);
    auto token = builder_->getToken(payload);

    if (!collection_->has(token))
        collection_->add(token);

    auto [oldest_token, oldest_time] = collection_->getOldest();
    timer_->start(oldest_token, oldest_time);

    return token;
};

std::pair<std::string, std::string> JWTAuthenticationManager::getInfo(const jwt_token &token)
{
    auto payload = builder_->getPayload(token);
    return payload.getInfo();
};