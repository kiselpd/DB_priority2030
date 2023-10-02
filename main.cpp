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
    return EXIT_SUCCESS;
};