#ifndef NETWORK_H
#define NETWORK_H

#include "chessPiece.h"


void startServer(int port);
void startClient(std::string& ip, int port);

#endif //NETWORK_H