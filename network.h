#ifndef NETWORK_H
#define NETWORK_H

#include "chessPiece.h"
#include "menu.h"
#include "globals.h"
#include <boost/asio.hpp>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <cstdint>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdlib>

using boost::asio::ip::udp;


//P2P functions
void punchHole(std::string& ip, int port, udp::socket& socket, boost::asio::io_context& io_context);
void hitStun(std::string& ip, int& port, udp::socket& socket, boost::asio::io_context& io_context);
void clearSocketBuffer(udp::socket& socket);
void sendMessages(udp::socket& socket, const udp::endpoint& peer_endpoint);

//LAN functions
void printLocalIP();
void getIpForLan(std::string& ip);
void broadcastIP(udp::socket& socket, boost::asio::io_context& io_context, int port, std::string local_ip);
void listenForLan(udp::socket& socket, boost::asio::io_context& io_context, int port, std::string& local_ip, std::string& peer_ip);


       /*@@@@@@@@@@@** queue functions  **@@@@@@@@@@@@@@*/

// listens for user input, adds it to appropriate queue
void ingestLocalData(bool& currentColor, bool& localColor, udp::socket& socket, udp::endpoint& peer_endpoint, std::queue<std::string>& moveQueue,
                     std::queue<std::string>& chatQueue, std::mutex& moveMutex, std::mutex& chatMutex, std::condition_variable& queueCondVar);
//listens to socket, adds incoming data to appropriate queue
void ingestExternalData(bool& localColor, bool& drawOffered, udp::socket& socket, udp::endpoint& peer_endpoint, std::queue<std::string>& moveQueue,
                   std::queue<std::string>& chatQueue, std::mutex& moveMutex, std::mutex& chatMutex, std::condition_variable& queueCondVar);
//utilities for queueing
void enqueueString(std::queue<std::string>& queue, std::string item, std::mutex& mutex, std::condition_variable& condVar); 
void dequeueString(std::queue<std::string>& queue, std::string& item, std::mutex& mutex, std::condition_variable& condVar); 


//other
bool waitForDrawResponse(udp::socket& socket, udp::endpoint& peer_endpoint);


#endif //NETWORK_H