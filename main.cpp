#include <iostream>
#include "client_acceptor.h"
#include <chrono>
#include <thread>
#include "db_backend.h"
#include "connection_pool.h"

int main() { 
   boost::asio::io_service io_service;
   std::shared_ptr<DBConnectionPool> pool = std::make_shared<DBConnectionPool>();
   DBConnectionOption option;
   pool->createPool(option);
   std::cout << pool->getFreeCount() << std::endl;
   std::shared_ptr<DBBackend> backend = std::make_shared<DBBackend>(pool);
   ClientAcceptor acceptro(backend, io_service, 2004);
   acceptro.start();
};