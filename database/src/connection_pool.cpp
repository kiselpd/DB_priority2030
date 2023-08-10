#include "connection_pool.h"

// DBConnectionPool
void DBConnectionPool::setOption(const DBConnectionOption& db_option){
    this->db_option_ = db_option;
};

size_t DBConnectionPool::createPool(const size_t& pool_size = DEFAULT_POOL_SIZE){
    for (size_t i = 0; i < pool_size; i++){
        std::shared_ptr<DBConnection> conn;
        auto error = conn->connect(this->db_option_);
        if(!error)
            this->pool_.emplace(conn);
        else
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};

size_t DBConnectionPool::getSize() const{
    std::lock_guard locker(this->m_mutex_);
    return this->pool_.size();
};

std::shared_ptr<DBConnection> DBConnectionPool::getFreeConnection(){
    std::unique_lock locker(this->m_mutex_);
    while(this->pool_.empty()){
        this->m_condition_.wait(locker);
    }

    auto free_connection = this->pool_.front();
    this->pool_.pop();

    return  free_connection;
};

void DBConnectionPool::setFreeConnection(std::shared_ptr<DBConnection> free_connection){
    std::unique_lock locker(m_mutex_);
    this->pool_.push(free_connection);
    locker.unlock();
    m_condition_.notify_one();
};