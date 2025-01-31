#ifndef MAIN_H
#define MAIN_H

// #include "board.h"
#include "menu.h"
#include "network.h"
#include "globals.h"
#include "settingsConfig.h"
#include "stockFishInterface.h"


void startLocalGame(std::string& opening);
void startOnlineGame(bool& turnRef, bool localColor, bool& drawOffered, bool&drawAccepted, bool& running, udp::socket& socket, udp::endpoint& peer_endpoint, float& turnNum, bool& opponentReady, GameBoard& board, char& gameResult, std::string& opening);
void startHost();
void startJoin();
#endif //MAIN_H