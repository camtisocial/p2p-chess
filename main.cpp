
#include "main.h"

int main(int argc, char** argv) {

    GameBoard board;
    nlohmann::json boardState;
    //0 = white to play, 1 = black to play
    bool to_play = 0;
    int turn = 0;
    
    std::cout << std::endl;
    std::string q = "";
    while(q != "q") {
        std::cout << R"(
         _                   
     ___| |__   ___  ___ ___  
    / __| '_ \ / _ \/ __/ __|
   | (__| | | |  __/\__ \__ \
    \___|_| |_|\___||___/___/)";


        std::cout << "\n\n\n";
        board.printBoard();
        std::cout << "\n\n";
        std::cout << "   Enter move: ";
        std::cout.flush();
        std::getline(std::cin,  q);
        if (q != "q") {
            if (board.movePiece(q, to_play)) {
                turn++;
                to_play = !to_play;
                board.saveBoardState(turn, to_play, boardState);
                std::cout << boardState.dump(4) << std::endl;
            };
            
        }
        std::cout << "\n";
        system("cls");
    }

	return 0;
}

