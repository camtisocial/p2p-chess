
#include "board.h"

GameBoard::GameBoard() {
    int color = 1;
    for (int i{}; i<7; i++) {
        for (int j{}; j<7; j++) {
            Square *newSquare = new Square;

            // 0 = black : 1 = white
            if (color%2 == 0) {
                newSquare->color = 0;
            } else {
                newSquare->color = 1;
            }
            newSquare->current = pawn

            //board[i][j] = newSquare;
            //color++;
        }
    }
}

GameBoard::~GameBoard() {
    for (int i{}; i<7; i++) {
        for (int j{}; j<7; j++) {
            delete board[i][j];
        }
    }
}


void GameBoard::printBoard() {
    for (int i{}; i<7; i++) {
        std::cout << std::endl;
        for (int j{}; j<7; j++) {
            std::cout << board[i][j]->current.name << " ";
        }
    }
}
