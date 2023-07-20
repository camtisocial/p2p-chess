
#include "main.h"

int main(int argc, char** argv) {

   // std::cout << "creating new board...";
    GameBoard board;
    //std::cout << "done"<< std::endl << std::endl;

  //  std::cout << "displaying board...\n " << std::endl;
//    board.printBoard();
    std::cout << std::endl;

    std::string q = "";
    while(q != "q") {
        board.printBoard();
        std::cout << "\nEnter move: ";
        std::cin >> q;
        std::cout << "\n";

    }


	return 0;
}

