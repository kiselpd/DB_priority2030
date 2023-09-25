#ifndef BACKEND_H
#define BACKEND_H

#include <pqxx/pqxx>

#include "connection_pool.h"
#include "request.h"

class DBBackend
{
public:
    DBBackend(std::shared_ptr<DBConnectionPool> pool);
    pqxx::result doRequest(std::shared_ptr<DBBaseRequest> request);

private:
    std::shared_ptr<DBConnectionPool> pool_;
};

#endif /*BACKEND_H*/