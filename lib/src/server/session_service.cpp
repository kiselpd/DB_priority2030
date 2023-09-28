#include "session_service.h"

// SessionService
SessionService::SessionService(std::shared_ptr<DBBackend> db_backend, std::shared_ptr<JWTAuthenticationManager> jwt_manager) : db_backend_(db_backend),
                                                                                                                               jwt_manager_(jwt_manager),
                                                                                                                               sessions_(std::make_shared<std::vector<std::shared_ptr<Session>>>()){};

void SessionService::recive(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr)
{
    this->createSession(socket_ptr);
};

void SessionService::createSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr)
{
    std::lock_guard<std::mutex> locker(mut_);
    auto new_session = std::make_shared<Session>(socket_ptr, db_backend_, jwt_manager_);
    sessions_->push_back(new_session);
    new_session->start(sessions_);
};