#ifndef NETWORK_H
#define NETWORK_H

#include "chessPiece.h"
#include "menu.h"
#include "globals.h"
#include "settingsConfig.h"
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

//LAN functions
void printLocalIP();
void getIpForLan(std::string& ip);
void broadcastIP(udp::socket& socket, boost::asio::io_context& io_context, int port, std::string local_ip);
void listenForLan(udp::socket& socket, boost::asio::io_context& io_context, int port, std::string& local_ip, std::string& peer_ip);

//Shared Functions
void sendMessages(udp::socket& socket, const udp::endpoint& peer_endpoint);
void clearSocketBuffer(udp::socket& socket);
void listenForColor(udp::socket& socket, udp::endpoint& peer_endpoint, bool& localColor, boost::asio::io_context& io_context);


       /*@@@@@@@@@@@** queue functions  **@@@@@@@@@@@@@@*/

// listens for user input, adds it to appropriate queue
void ingestLocalData(bool& currentColor, bool& localColor, bool& drawOffered, bool& drawAccepted, bool& drawOfferReceived, udp::socket& socket, udp::endpoint& peer_endpoint, std::queue<std::string>& moveQueue,
                     std::queue<std::string>& chatQueue, std::mutex& moveMutex, std::mutex& chatMutex, std::condition_variable& queueCondVar, bool& running, float& turnNumber, int& labelsOn, bool& lastMoved);
//listens to socket, adds incoming data to appropriate queue
void ingestExternalData(bool& localColor, bool& drawOffered, bool& drawAccepted, bool& drawOfferReceived, udp::socket& socket, udp::endpoint& peer_endpoint, std::queue<std::string>& moveQueue,
                   std::queue<std::string>& chatQueue, std::mutex& moveMutex, std::mutex& chatMutex, std::condition_variable& queueCondVar, bool& running, float& turnNumber, bool& opponentReady);
//utilities for queueing
void enqueueString(std::queue<std::string>& queue, std::string item, std::mutex& mutex, std::condition_variable& condVar); 
void dequeueString(std::queue<std::string>& queue, std::string& item, std::mutex& mutex, std::condition_variable& condVar); 

#endif //NETWORK_H
