
#include "board.h"

GameBoard::GameBoard() {
    vector<Square*> tmpVec;
    int color = 1;
    for (int i{}; i<8; i++) {
        for (int j{}; j<8; j++) {

            Square *newSquare = new Square;
            //ChessPiece *newPiece = new ChessPiece; 
            //newSquare->current = *newPiece;


            // 0 = black : 1 = white
            if (color%2 == 0) {
                newSquare->color = 0;
            } else {
                newSquare->color = 1;
            }

            tmpVec.push_back(newSquare);
            color++;

            if (j == 7) {
                board.push_back(tmpVec);
            }

        }
    }
    std::cout << "This ---> " << color << std::endl;
}

GameBoard::~GameBoard() {
    for (int i{}; i<8; i++) {
        for (int j{}; j<8; j++) {
            delete board[i][j];
            //delete board[j];
        }
    }
}

        


void GameBoard::printBoard() {
    for (int i{}; i<8; i++) {
        std::cout << std::endl;
        for (int j{}; j<8; j++) {
            std::cout << board[i][j]->current.name << " ";
        }
    }
}
