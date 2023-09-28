#ifndef SESSION_SERVICE_H
#define SESSION_SERVICE_H

#include "mediator.h"
#include "backend.h"
#include "jwt_manager.h"
#include "session.h"

#include <mutex>
#include <vector>

#include "boost/asio.hpp"

class SessionService : public SessionServiceColleague
{
public:
    SessionService(std::shared_ptr<DBBackend> db_backend, std::shared_ptr<JWTAuthenticationManager> jwt_manager);

    void recive(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr) final;
    void createSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr);

private:
    std::shared_ptr<DBBackend> db_backend_;
    std::shared_ptr<JWTAuthenticationManager> jwt_manager_;
    std::shared_ptr<std::vector<std::shared_ptr<Session>>> sessions_;

    std::mutex mut_;
};

#endif /*SESSION_SERVICE_H*/