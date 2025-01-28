#ifndef MAIN_H
#define MAIN_H

// #include "board.h"
#include "menu.h"
#include "network.h"
#include "globals.h"
#include "settingsConfig.h"


void startLocalGame();
void startOnlineGame(bool& turnRef, bool localColor, bool& drawOffered, bool&drawAccepted, bool& running, udp::socket& socket, udp::endpoint& peer_endpoint, float& turnNum, bool& opponentReady, GameBoard& board, char& gameResult);
void startHost();
void startJoin();
#endif //MAIN_H