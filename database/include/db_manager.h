#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "connection_pool.h"

class DBManager
{
public:
    size_t createConnectionPool(const DBConnectionOption& option, const size_t& connection_count);
    size_t clearConnectionPool();

    size_t addConnection(const size_t& conn_number = 1);
    size_t deleteConnection(const size_t& conn_number = 1);

    size_t getFreeConnectionCount() const;
    size_t getAllConnectionCount() const;
    DBConnectionOption getConnectionOption() const;

    pqxx::result doRequest(std::shared_ptr<BaseDBRequest> request);

private:
    std::shared_ptr<DBConnectionPool> conn_pool_;
};

#endif /*DB_MANAGER_H*/