#include "connection_pool.h"

// DBConnectionPool
DBConnectionPool::DBConnectionPool(const DBConnectionOption &option, const size_t &pool_size) : option_(option), pool_size_(pool_size){};

size_t DBConnectionPool::createPool()
{
    for (size_t i = 0; i < this->pool_size_; i++)
    {
        auto error = this->addConnection();
        if (error)
        {
            this->pool_size_ = i;
            break;
        }
    }
    return this->pool_size_;
};

void DBConnectionPool::clearPool()
{
    while (this->pool_size_)
    {
        this->deleteConnection();
        this->pool_size_--;
    }
};

std::shared_ptr<DBConnection> DBConnectionPool::getFreeConnection()
{
    std::unique_lock locker(this->m_mutex_);
    while (this->pool_.empty())
    {
        this->m_condition_.wait(locker);
    }

    auto free_connection = this->pool_.front();
    this->pool_.pop();

    return free_connection;
};

void DBConnectionPool::setFreeConnection(std::shared_ptr<DBConnection> free_connection)
{
    std::unique_lock locker(this->m_mutex_);
    this->pool_.push(free_connection);
    locker.unlock();
    m_condition_.notify_one();
};

size_t DBConnectionPool::deleteConnection(const size_t &connections_count)
{
    for (size_t i = 0; i < connections_count; i++)
        if (pool_size_)
        {
            auto free_conn = this->getFreeConnection();
            this->pool_size_--;
        }

    return this->pool_size_;
};

size_t DBConnectionPool::addConnection(const size_t &connections_count)
{
    for (size_t i = 0; i < connections_count; i++)
    {
        std::shared_ptr<DBConnection> new_conn = std::make_shared<DBConnection>();
        auto error = new_conn->connect(this->getOption());
        if (!error)
        {
            this->setFreeConnection(new_conn);
            this->pool_size_++;
        }
    }

    return this->pool_size_;
};

DBConnectionOption DBConnectionPool::getOption() const
{
    return this->option_;
};

size_t DBConnectionPool::getPoolSize() const
{
    return this->pool_size_;
};

size_t DBConnectionPool::getFreeConectionCount()
{
    std::lock_guard<std::mutex> locker(this->m_mutex_);
    return this->pool_.size();
};