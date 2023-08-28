#include <iostream>
#include "client_acceptor.h"
#include "db_backend.h"
#include "connection_pool.h"

namespace default_db_info
{
   const std::string host = "127.0.0.1";
   const std::string port = "5432";
   const std::string user = "postgres";
   const std::string password = "qwerty";
   const std::string name = "Priority2030";

   const size_t pool_size = 10;
};

DBConnectionOption create_option(){
   DBConnectionOption option(
      default_db_info::host,
      default_db_info::port,
      default_db_info::user,
      default_db_info::password,
      default_db_info::name);

      return option;
};

std::shared_ptr<DBConnectionPool> create_connection_pool(const DBConnectionOption& option, const size_t& pool_size){
   std::shared_ptr<DBConnectionPool> pool = std::make_shared<DBConnectionPool>();
   pool->createPool(option, pool_size);
   return pool;
};



void start(const short& port){
   boost::asio::io_service io_service;
   auto option = create_option();
   auto pool = create_connection_pool(option, default_db_info::pool_size);
   std::shared_ptr<DBBackend> backend = std::make_shared<DBBackend>(pool);
   ClientAcceptor acceptor(backend, io_service, port);
   acceptor.start();
};

int main(int argc, char *argv[]) { 
   if(argc <= 1){
      std::cout << "Incorrect port!" << std::endl;
      return EXIT_FAILURE;
   }

   short port = std::atoi(argv[1]);
   
   start(port);
};