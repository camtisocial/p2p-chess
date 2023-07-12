
#include "main.h"

int main(int argc, char** argv) {

    std::cout << "creating new board..." << std::endl;
    GameBoard board;
    std::cout << "done"<< std::endl << std::endl;

    std::cout << "displaying empty board " << std::endl;
    board.printBoard();


	return 0;
}

