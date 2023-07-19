
#include "board.h"

GameBoard::GameBoard() {
    vector<Square*> tmpVec;
    int color = 1;
    for (int i{}; i<8; i++) {
        for (int j{}; j<8; j++) {
            Square *newSquare = new Square;
            ChessPiece *newPiece = new Pawn; 
            std::cout << newPiece->getName();
            newSquare->current = newPiece;
            std::cout << newSquare->current->getName();


            // 0 = black : 1 = white
            if (color%2 == 0) {
                newSquare->color = 0;
            } else {
                newSquare->color = 1;
            }

            tmpVec.push_back(newSquare);

            if (j == 7) {
                board.push_back(tmpVec);
                color++;
            }

        }
    }
    std::cout << "This ---> " << color << std::endl;
}

GameBoard::~GameBoard() {
    for (int i{0}; i<8; i++) {
        delete (board[0][i]);
        }
    //This is a pain, I can delete the first row, but as soon as I try
    //to delete the second, it causes a double free error. No clue why. 
    /*
    for (int i{}; i<8; i++) {
        for (int j{}; j<8; j++) {
            delete board[i][j];
            if (j == 7) {
                board[i].clear();
            }
            //delete board[j];
        }
    }
    for (auto b : board) {
        for (int i{}; i<1; i++) {
            std::cout << "\n" << i+1;
            delete b[i];
        }
    }
    std::cout << board[0][0]->current.name << "<--- ";
    for (auto b : board) {
            std::cout << std::endl;
            std::cout << "   ";
        for (int t{}; t<8; t++) {
            std::cout << b[t]->current.name << " ";
        }
    }
    */
}

void GameBoard::printBoard() {
    for (auto b : board) {
            std::cout << std::endl;
            std::cout << "   ";
        for (int i{}; i<8; i++) {

            std::cout << b[i]->current->getName() << " ";
            //std::cout << b[i]->current.name << " ";
        }
    }
}

/*
void GameBoard::printBoard() {
    for (int i{}; i<8; i++) {
        std::cout << std::endl;
        for (int j{}; j<8; j++) {
            std::cout << board[i][j]->current.name << " ";
        }
    }
}
void GameBoard::printBoard() {
    for (auto p : board) {
        std::cout << std::endl;
        for (auto b : p) {
            std::cout << b->current.name << " ";
        }
    }
}
 * */
