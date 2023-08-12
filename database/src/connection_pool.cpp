#include "connection_pool.h"

// DBConnectionPool
size_t DBConnectionPool::createPool(const DBConnectionOption& db_option, const size_t& pool_size = DEFAULT_POOL_SIZE){
    for (size_t i = 0; i < pool_size; i++){
        auto error = this->addConnection();
        if(error)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};

void DBConnectionPool::clearPool(){
    while (this->connection_count_)
        this->deleteConnection();
};

size_t DBConnectionPool::getAllCount() const{
    return this->connection_count_;
};

size_t DBConnectionPool::getFreeCount() const{
    std::lock_guard locker(this->m_mutex_);
    return this->pool_.size();
};

DBConnectionOption DBConnectionPool::getOption() const{
    return this->option_;
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

size_t DBConnectionPool::deleteConnection(){
    if(this->connection_count_){
        this->getFreeConnection();
        this->connection_count_--;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
};

size_t DBConnectionPool::addConnection(){
    auto new_conn = std::make_shared<DBConnection>();
    auto error = new_conn->connect(this->option_);
    if(!error){
        this->setFreeConnection(new_conn);
        this->connection_count_++;
    }
    return error;
};