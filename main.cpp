// #include <iostream>
// #include "client_acceptor.h"
// #include "backend.h"
// #include "connection_pool.h"

// // #include <jwt_builder.h>

// // std::shared_ptr<DBConnectionPool> create_connection_pool(const DBConnectionOption& option, const size_t& pool_size){
// //    std::shared_ptr<DBConnectionPool> pool = std::make_shared<DBConnectionPool>();
// //    pool->createPool(option, pool_size);
// //    return pool;
// // };

// // void start(const short& port){
// //    boost::asio::io_service io_service;
// //    auto option = create_option();
// //    auto pool = create_connection_pool(option, default_db_info::pool_size);
// //    std::shared_ptr<DBBackend> backend = std::make_shared<DBBackend>(pool);
// //    ClientAcceptor acceptor(backend, io_service, port);
// //    acceptor.start();
// // };

// int main(int argc, char *argv[]) { 
//    // if(argc <= 1){
//    //    std::cout << "Incorrect port!" << std::endl;
//    //    return EXIT_FAILURE;
//    // }

//    // short port = std::atoi(argv[1]);
   
//    // start(port);
// };

#include <iostream>
#include "jwt/jwt.hpp"

int main() {
  using namespace jwt::params;

  auto key = "secret"; //Secret to use for the algorithm
  //Create JWT object
  jwt::jwt_object obj{algorithm("HS256"), payload({{"some", "payload"}}), secret(key)};

  //Get the encoded string/assertion
  auto enc_str = obj.signature();
  std::cout << enc_str << std::endl;

  //Decode
  auto dec_obj = jwt::decode(enc_str, algorithms({"HS256"}), secret(key));
  std::cout << dec_obj.header() << std::endl;
  std::cout << dec_obj.payload() << std::endl;

  return 0;
}