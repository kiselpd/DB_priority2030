#include <iostream>
#include "client_acceptor.h"
#include <chrono>
#include <thread>

int main() { 
   boost::asio::io_service io_service;
   ClientAcceptor acceptro(io_service, 2006);
   acceptro.start();
};