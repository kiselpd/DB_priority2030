#ifndef SERVER_H
#define SERVER_H

#include "db_manager.h"
#include "acceptor.h"

class Server
{
public:

private:
    std::shared_ptr<DBManager> db_manager_;
    std::shared_ptr<ClientAcceptor> client_acceptor_;
};
#endif /*SERVER_H*/