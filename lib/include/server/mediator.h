#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "boost/asio.hpp"

class BaseColleague;

class Mediator : virtual public std::enable_shared_from_this<Mediator>
{
public:
    Mediator(std::shared_ptr<BaseColleague> acceptor_service, std::shared_ptr<BaseColleague> session_service);
    virtual ~Mediator(){};

    void registerColleagues();
    void send(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr);

private:
    std::weak_ptr<BaseColleague> acceptor_service_;
    std::weak_ptr<BaseColleague> session_service_;
};

class BaseColleague
{
public:
    virtual void send(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr){};
    virtual void recive(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr){};
    virtual void registerMediator(std::shared_ptr<Mediator> mediator){};
};

class AcceptorServiceColleague : public BaseColleague
{
public:
    void send(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr) final;
    void registerMediator(std::shared_ptr<Mediator> mediator) final;

private:
    std::shared_ptr<Mediator> mediator_;
};

class SessionServiceColleague : public BaseColleague
{
public:
    void registerMediator(std::shared_ptr<Mediator> mediator) final;

private:
    std::shared_ptr<Mediator> mediator_;
};

#endif /*MEDIATOR_H*/