#include "mediator.h"

// Mediator
Mediator::Mediator(std::shared_ptr<BaseColleague> acceptor_service, std::shared_ptr<BaseColleague> session_service) : acceptor_service_(acceptor_service),
                                                                                                                      session_service_(session_service){};

void Mediator::registerColleagues()
{
    acceptor_service_.lock()->registerMediator(this->shared_from_this());
    session_service_.lock()->registerMediator(this->shared_from_this());
};

void Mediator::send(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr)
{
    auto shared_service = session_service_.lock();

    if (shared_service)
        shared_service->recive(socket_ptr);
};

// AcceptorServiceColleague
void AcceptorServiceColleague::send(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr)
{
    if (mediator_)
        mediator_->send(socket_ptr);
};

void AcceptorServiceColleague::registerMediator(std::shared_ptr<Mediator> mediator)
{
    mediator_ = mediator;
};

// SessionServiceColleague
void SessionServiceColleague::registerMediator(std::shared_ptr<Mediator> mediator)
{
    mediator_ = mediator;
};
