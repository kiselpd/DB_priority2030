#ifndef JWT_MANAGER_H
#define JWT_MANAGER_H

#include <map>
#include <mutex>

#include "jwt_builder.h"
#include "jwt_collection.h"

class JWTAuthenticationManager
{
public:
    JWTAuthenticationManager(std::shared_ptr<boost::asio::io_service> io_ptr, const std::vector<std::string> &list_algo, const std::string &secret);

    bool isAuthenticated(const jwt_token &token);
    jwt_token addNewJWT(const std::string &user_id, const std::string &user_role);
    std::pair<std::string, std::string> getInfo(const jwt_token &token);

private:
    std::shared_ptr<JWTCollection> collection_;
    std::shared_ptr<JWTStatusTimer> timer_;
    std::shared_ptr<JWTBuilder> builder_;
};

#endif /*JWT_MANAGER_H*/