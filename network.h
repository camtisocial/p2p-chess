#ifndef NETWORK_H
#define NETWORK_H

#include "chessPiece.h"

void serializeBoard(vector<vector<std::shared_ptr<ChessPiece>>> board, int turnNum, int playerTurn, nlohmann::json &jsonObject);
void deSerializeBoard(vector<vector<std::shared_ptr<ChessPiece>>> board, int turnNum, int playerTurn, nlohmann::json &jsonObject);

#endif //NETWORK_H