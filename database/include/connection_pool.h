#ifndef CONNECTION_POOL
#define CONNECTION_POOL

#include <queue>
#include <mutex>
#include <condition_variable>
#include "connection.h"

const size_t DEFAULT_POOL_SIZE = 10;

class DBConnectionPool
{
public:
    void setOption(const DBConnectionOption& db_option);
    size_t createPool(const size_t& pool_size = DEFAULT_POOL_SIZE);

    size_t getSize() const;
    std::shared_ptr<DBConnection> getFreeConnection();

    void setFreeConnection(std::shared_ptr<DBConnection> free_connection);

private:
    DBConnectionOption db_option_;
    std::queue<std::shared_ptr<DBConnection>> pool_;
    std::condition_variable m_condition_;
    std::mutex m_mutex_;
};

#endif /*CONNECTION_POOL*/