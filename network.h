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
void receiveMessages(udp::socket& socket); 

//LAN functions
void printLocalIP();
void getIpForLan(std::string& ip);
void broadcastIP(udp::socket& socket, boost::asio::io_context& io_context, int port, std::string local_ip);
void listenForLan(udp::socket& socket, boost::asio::io_context& io_context, int port, std::string& local_ip, std::string& peer_ip);


void addMessageToQueue(const std::string& message); 
void startServer(int port);
void startClient(std::string& ip, int port);
std::string getMessageFromQueue();

#endif //NETWORK_H