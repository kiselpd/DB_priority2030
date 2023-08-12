#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "connection.h"

const size_t DEFAULT_POOL_SIZE = 10;

class DBConnectionPool
{
public:
    size_t createPool(const DBConnectionOption& db_option, const size_t& pool_size = DEFAULT_POOL_SIZE);
    void clearPool();

    size_t getAllCount() const;
    size_t getFreeCount() const;
    DBConnectionOption getOption() const;

    std::shared_ptr<DBConnection> getFreeConnection();
    void setFreeConnection(std::shared_ptr<DBConnection> free_connection);

    size_t deleteConnection();
    size_t addConnection();
    
private:
    DBConnectionOption option_;
    size_t connection_count_;
    std::queue<std::shared_ptr<DBConnection>> pool_;
    std::condition_variable m_condition_;
    std::mutex m_mutex_;
};

#endif /*CONNECTION_POOL_H*/