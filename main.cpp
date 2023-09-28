// std::shared_ptr<DBConnectionPool> create_connection_pool(const DBConnectionOption& option, const size_t& pool_size){
//    std::shared_ptr<DBConnectionPool> pool = std::make_shared<DBConnectionPool>();
//    pool->createPool(option, pool_size);
//    return pool;
// };

// void start(const short& port){
//    boost::asio::io_service io_service;
//    auto option = create_option();
//    auto pool = create_connection_pool(option, default_db_info::pool_size);
//    std::shared_ptr<DBBackend> backend = std::make_shared<DBBackend>(pool);
//    ClientAcceptor acceptor(backend, io_service, port);
//    acceptor.start();
// };

int main(int argc, char *argv[]) { 

};