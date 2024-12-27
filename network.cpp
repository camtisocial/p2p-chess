#include "network.h"


void serializeBoard(vector<vector<std::shared_ptr<ChessPiece>>> board, int turnNum, int playerTurn, nlohmann::json &jsonObject) {
    jsonObject["turnNum"] = turnNum;
    jsonObject["playerTurn"] = playerTurn;

    for (const auto& row : board) {
        for (const auto& piece : row) {
            if (piece) {
                jsonObject["board"].push_back({
                    {"type", piece->getName()},
                    {"row", piece->getRow()},
                    {"column", piece->getColumn()},
                    {"color", piece->color}
                });
            } else {
                jsonObject["board"].push_back(nullptr);
            }
        }
    }

}

void deSerializeBoard(vector<vector<std::shared_ptr<ChessPiece>>> board, int turnNum, int playerTurn, nlohmann::json &jsonObject);