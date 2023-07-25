#include <sstream>
#include "board.h"

GameBoard::GameBoard() {
    vector<Square*> tmpVec;
    int counter = 0;
///////////////////////////////// SETTING BLACK PIECES\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\;
    for (int i{}; i<8; i++) {
        Square *newSquare = new Square;
        ChessPiece *newPiece;

        if (i == 0 || i == 7) {
            newPiece = new Rook; 
        }
        else if (i == 1 || i == 6) {
            newPiece = new Knight; 
        }
        else if (i == 2 || i == 5) {
            newPiece = new Bishop; 
        }
        else if (i == 3) {
            newPiece = new Queen; 
        } else {
            newPiece = new King; 
        }

        newPiece->color = 'B';
        newSquare->current = newPiece;
        tmpVec.push_back(newSquare);
    }

    board.push_back(tmpVec);
    tmpVec.clear();

    for (int j{}; j<8; j++) {
        Square *newSquare = new Square;
        ChessPiece *newPiece = new Pawn; 
        newPiece->color = 'B';
        newSquare->current = newPiece;
        tmpVec.push_back(newSquare);
    }

    board.push_back(tmpVec);
    tmpVec.clear();

///////////////////////////////// SETTING EMPTY SPACES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\;
    for (int i{}; i<4; i++) {
        for (int j{}; j<8; j++) {
            Square *newSquare = new Square;
            ChessPiece *newPiece = new ChessPiece; 
            newSquare->current = newPiece;

            tmpVec.push_back(newSquare);

            if (j == 7) {
                board.push_back(tmpVec);
                tmpVec.clear();
            }
        }
    }

///////////////////////////////// SETTING WHITE PIECES\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\;
    for (int i{}; i<8; i++) {
        Square *newSquare = new Square;
        ChessPiece *newPiece = new Pawn; 
        newPiece->color = 'W';
        newSquare->current = newPiece;
        tmpVec.push_back(newSquare);
    }

    board.push_back(tmpVec);
    tmpVec.clear();

    for (int i{}; i<8; i++) {
        Square *newSquare = new Square;
        ChessPiece *newPiece;

        if (i == 0 || i == 7) {
            newPiece = new Rook; 
        }
        else if (i == 1 || i == 6) {
            newPiece = new Knight; 
        }
        else if (i == 2 || i == 5) {
            newPiece = new Bishop; 
        }
        else if (i == 3) {
            newPiece = new Queen; 
        } else {
            newPiece = new King; 
        }

        newPiece->color = 'W';
        newSquare->current = newPiece;
        tmpVec.push_back(newSquare);
    }

    board.push_back(tmpVec);
}

GameBoard::~GameBoard() {
    for (int i{0}; i<8; i++) {
        delete (board[0][i]);
        }
}

void GameBoard::printBoard() {
    for (auto b : board) {
            std::cout << "        ";
        for (int i{}; i<8; i++) {
            char tmp = b[i]->current->getName();

            if (b[i]->current->color == 'B') {
                std::cout << "\x1B[92m" << tmp << "\033[0m" <<" ";
            }
            else if(b[i]->current->color == 'W') {
                std::cout << "\x1B[91m" << tmp << "\033[0m" <<" ";
            } else {
                std::cout << "\x1B[90m" << tmp << "\033[0m" <<" ";

            }
        }
        std::cout << std::endl;
    }
}

bool GameBoard::movePiece(std::string u_input) {



    /*
    std::string from, to;
    std::stringstream s(u_input);
    s>>from>>to;
    //std::cout << from[0];
    std::cout << moveMap[from[0]] << from[1] << "-" << moveMap[to[0]] << to[1] << std::endl;
    //std::cout << moveMap[0];
    //<< from[1] << "-" << moveMap[to[0]] << to[1] << std::endl;
    */

    return 0;


}


























