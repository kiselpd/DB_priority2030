#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "connection.h"

class DBConnectionPool
{
public:
    size_t createPool(const DBConnectionOption& option, const size_t& pool_size);
    void clearPool();

    std::shared_ptr<DBConnection> getFreeConnection();
    void setFreeConnection(std::shared_ptr<DBConnection> free_connection);

    size_t deleteConnection();
    size_t addConnection();

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