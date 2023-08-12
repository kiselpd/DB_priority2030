#include "db_manager.h"

// DBManager
size_t DBManager::createConnectionPool(const DBConnectionOption& option, const size_t& connection_count){
    auto new_conn_pool = std::make_shared<DBConnectionPool>();
    auto error = new_conn_pool->createPool(option, connection_count);
    if(!error)
        this->conn_pool_ = new_conn_pool;
    return error;
};

size_t DBManager::clearConnectionPool(){
    conn_pool_->clearPool();
    return (conn_pool_->getAllCount()) ? (1) : (0);
};

size_t DBManager::addConnection(const size_t& conn_number = 1){
    for (size_t i = 0; i < conn_number; i++){
        auto error = conn_pool_->addConnection();
        if(error)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};

size_t DBManager::deleteConnection(const size_t& conn_number = 1){
    for (size_t i = 0; i < conn_number; i++){
        auto error = conn_pool_->deleteConnection();
        if(error)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};

size_t DBManager::getFreeConnectionCount() const{
    return conn_pool_->getFreeCount();
};

size_t DBManager::getAllConnectionCount() const{
    return conn_pool_->getAllCount();
};

DBConnectionOption DBManager::getConnectionOption() const{
    return conn_pool_->getOption();
};

pqxx::result DBManager::doRequest(std::shared_ptr<BaseDBRequest> request){
    auto free_conn = conn_pool_->getFreeConnection();
    auto result = free_conn->doRequest(request);
    conn_pool_->setFreeConnection(free_conn);
    return result;
};
