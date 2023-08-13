#ifndef SERVER_H
#define SERVER_H

#include "db_manager.h"
#include "client_acceptor.h"

class Server
{
public:
    
private:
    size_t addDBManager_(std::shared_ptr<DBManager> db_manager);
    size_t addClientAcceptor_();

    std::shared_ptr<DBManager> db_manager_;
    std::shared_ptr<ClientAcceptor> client_acceptor_;

};
#endif /*SERVER_H*/