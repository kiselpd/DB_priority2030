#ifndef JWT_MANAGER_H
#define JWT_MANAGER_H

#include <map>
#include <mutex>
#include <jwt_builder.h>

class JWTCollection
{
public:
    bool has(const std::string& token) const;
    void add(const std::string& token);
    void remove(const std::string& token);
    
private:
    std::map<std::string> token_set_;
    std::mutex mut_;
};


class JWTStatusTimer
{
public:

private:
};


class JWTAuthenticationManager
{
public:
    bool isAuthenticated(const std::string& token) const;


private:
    JWTCollection collection_;
    JWTBuilder builder_;
};

#endif /*JWT_MANAGER_H*/