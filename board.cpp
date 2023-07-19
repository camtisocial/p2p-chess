
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
            //std::cout << newPiece->color;
            //std::cout << newPiece->getName();
            //std::cout << newSquare->current->getName();

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
            std::cout << "   ";
        for (int i{}; i<8; i++) {
            std::cout << b[i]->current->getName() << " ";
        }
        std::cout << std::endl;
    }
}
