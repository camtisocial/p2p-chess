#ifndef STOCKFISHINTERFACE_H
#define STOCKFISHINTERFACE_H

#include <boost/process.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <mutex>

void getStockFishEval(std::string& fen, std::string& evaluatedPosition, std::string& bestMove, std::string& stockfish_path, int depth);
void evalEachMove(std::string stockfish_path, std::vector<std::string>& evalHistory, std::vector<std::string>& moveHistory, int depth, int& turnNumber, std::mutex& mtx);


#endif //STOCKFISHINTERFACE_H
