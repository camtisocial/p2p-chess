
#include "main.h"

int main(int argc, char** argv) {

   // std::cout << "creating new board...";
    GameBoard board;
    int turn = 0;
    bool to_play = 0
    //std::cout << "done"<< std::endl << std::endl;

  //  std::cout << "displaying board...\n " << std::endl;
//    board.printBoard();
    std::cout << std::endl;

    std::string q = "";
    while(q != "q") {
        std::cout << R"(
         _                   
     ___| |__   ___  ___ ___  
    / __| '_ \ / _ \/ __/ __|
   | (__| | | |  __/\__ \__ \
    \___|_| |_|\___||___/___/9999 )";


        std::cout << "\n\n\n";
        board.printBoard();
        std::cout << "\n\n";
        std::cout << "   Enter move: ";
        std::cout.flush();
        std::getline(std::cin,  q);
        if (q != "q") {
            if (board.movePiece(q)) {
                turn++;
                to_play = !to_play;
            };
            
        }
        std::cout << "\n";
        //system("cls");
    }

	return 0;
}

