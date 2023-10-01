#include <iostream>

#include "server.h"

size_t start(std::shared_ptr<Server> server){
    ServerOption server_option;
    auto error = server_option.readConfig("../config.cfg");

    if (!error)
        error = server->start(server_option);
    else
        std::cout << "Incorrect format config.cfg!" << std::endl;

    return error;
};

void stop(std::shared_ptr<Server> server){
    std::string info = "Press to stop!";
    std::cout << std::endl << info << std::endl;
    std::cin >> info;

    server->stop();
};

void run()
{
    std::shared_ptr<Server> server = std::make_shared<Server>();
    auto error = start(server);

    if(!error){
        std::cout << "Server is running!" << std::endl;
        stop(server);
    }
    else
        std::cout << "Server is not running!" << std::endl;
};

int main()
{
    run();
    // DBConnectionOption op;
    // op._dbname = "priority2030";
    // op._host = "127.0.0.1";
    // op._password = "12345";
    // op._port = "5432";
    // op._user = "postgres";
    // std::shared_ptr<DBConnection> conn = std::make_shared<DBConnection>();
    // conn->connect(op);
    return EXIT_SUCCESS;
};