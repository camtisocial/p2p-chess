#include "stockFishInterface.h"


namespace bp = boost::process;

void getStockFishEval(std::string& fen, std::string& evaluatedPosition, std::string& bestMove, std::string& stockfish_path, int depth) {

    bp::ipstream stockfish_output;
    bp::opstream stockfish_input;
    bp::child stockfish(stockfish_path, bp::std_in < stockfish_input, bp::std_out > stockfish_output);


    stockfish_input << "uci\n";
    stockfish_input << "isready\n";
    stockfish_input << "position fen " << fen << "\n";
    stockfish_input << "go depth " << std::to_string(depth) <<"\n";
    stockfish_input.flush();

    std::string line;
    while (std::getline(stockfish_output, line)) {
        if (line.find("score cp") != std::string::npos) {
            std::istringstream iss(line);
            std::string token;
            while (iss >> token) {
                if (token == "cp") {
                    iss >> evaluatedPosition;
                    break;
                }
            }
        }
        if (line.find("bestmove") != std::string::npos) {
            std::istringstream iss(line);
            std::string token;
            iss >> token >> bestMove;  
            break;  
        }
    }

    stockfish.terminate();
    stockfish.wait();


}
