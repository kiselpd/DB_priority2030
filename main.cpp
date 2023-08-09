#include <iostream>
#include "connection.h"
#include "request.h"
#include <pqxx/pqxx>

int main() { 
   std::shared_ptr<DBConnection> conn{new DBConnection()};
   DBConnectionInfo info;
   size_t error;

   error = conn->connect(info);
   if(error)
      return EXIT_FAILURE;

   if(conn->isActive()){
      std::shared_ptr<AuthorizationSelectRequest> request{new AuthorizationSelectRequest("priority2030", "12345")};
      auto result = conn->doRequest(request);
      for(const auto &field : result.at(0)){
         std::cout << field << " ";
      }      
      std::cout << std::endl;
   }

   conn->disconnect();
};