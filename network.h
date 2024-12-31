#ifndef NETWORK_H
#define NETWORK_H

#include "chessPiece.h"
#include <boost/asio.hpp>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>


void startServer(int port);
void startClient(std::string& ip, int port);
void addMessageToQueue(const std::string& message); 
std::string getMessageFromQueue();

#endif //NETWORK_H