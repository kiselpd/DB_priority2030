#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "connection.h"

class DBConnectionPool
{
public:
    DBConnectionPool(const DBConnectionOption &option, const size_t &pool_size);

    size_t createPool();
    void clearPool();

    std::shared_ptr<DBConnection> getFreeConnection();
    void setFreeConnection(std::shared_ptr<DBConnection> free_connection);

    size_t deleteConnection(const size_t &connections_count = 1);
    size_t addConnection(const size_t &connections_count = 1);

    DBConnectionOption getOption() const;
    size_t getPoolSize() const;
    size_t getFreeConectionCount();

private:
    DBConnectionOption option_;
    size_t pool_size_;

    std::queue<std::shared_ptr<DBConnection>> pool_;
    std::condition_variable m_condition_;
    std::mutex m_mutex_;
};

#endif /*CONNECTION_POOL_H*/