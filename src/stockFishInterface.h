#ifndef STOCKFISHINTERFACE_H
#define STOCKFISHINTERFACE_H

#include <boost/process.hpp>
#include <iostream>
#include <string>
#include <sstream>

void getStockFishEval(std::string& fen, std::string& evaluatedPosition, std::string& bestMove, std::string& stockfish_path, int depth);


#endif //STOCKFISHINTERFACE_H