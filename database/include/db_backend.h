#ifndef DB_BACKEND_H
#define DB_BACKEND_H

#include "connection_pool.h"
#include "client_parser.h"

class DBBackend
{
public:
    DBBackend(std::shared_ptr<DBConnectionPool> pool);
    std::string doRequest(const std::string& json_request);
    
private:
    std::shared_ptr<DBConnectionPool> pool_;
    std::shared_ptr<ClientParser> parser_;
};

#endif /*DB_BACKEND_H*/