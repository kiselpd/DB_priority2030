#include "connection_pool.h"

// DBConnectionPool
size_t DBConnectionPool::createPool(const DBConnectionOption& db_option, const size_t& pool_size){
    this->option_ = db_option;
    for (size_t i = 0; i < pool_size; i++){
        auto error = this->addConnection();
        if(error)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};

void DBConnectionPool::clearPool(){
    while (this->pool_size_){
        this->deleteConnection();
        this->pool_size_--;
    }
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
    std::unique_lock locker(this->m_mutex_);
    this->pool_.push(free_connection);
    locker.unlock();
    m_condition_.notify_one();
};


size_t DBConnectionPool::deleteConnection(){
    if(pool_size_){
        auto free_conn = this->getFreeConnection();
        this->pool_size_--;
    }

    return this->pool_size_;
};


size_t DBConnectionPool::addConnection(){
    std::shared_ptr<DBConnection> new_conn = std::make_shared<DBConnection>();
    auto error = new_conn->connect(this->getOption());
    if(!error){
        this->setFreeConnection(new_conn);
        this->pool_size_++;
    }
    return error;
};

DBConnectionOption DBConnectionPool::getOption() const{return this->option_;};

size_t DBConnectionPool::getPoolSize() const{
    return this->pool_size_;
};

size_t DBConnectionPool::getFreeConectionCount(){
    std::lock_guard<std::mutex> locker(this->m_mutex_);
    return this->pool_.size();
};