#ifndef STOCKFISHINTERFACE_H
#define STOCKFISHINTERFACE_H

#include <boost/process.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <mutex>
#include <atomic>
#include <regex>

void getStockFishEval(std::string fen, std::string stockfish_path,
                      int depth, std::vector<std::string>& evalHistory, std::string& currentCentipawnEval);


#endif //STOCKFISHINTERFACE_H
