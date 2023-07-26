#include <sstream>
#include "board.h"

GameBoard::GameBoard() {
    vector<Square*> tmpVec;
    int counter = 0;
///////////////////////////////// SETTING BLACK PIECES\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\;
    for (int i{}; i<8; i++) {
        Square *newSquare = new Square;
        newSquare->row = 0;
        newSquare->column = i;
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
        newSquare->row = 1;
        newSquare->column = j;

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
            newSquare->row = i+2;
            newSquare->column = j;

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
        newSquare->row = 6;
        newSquare->column = i;

        ChessPiece *newPiece = new Pawn; 
        newPiece->color = 'W';
        newSquare->current = newPiece;
        tmpVec.push_back(newSquare);
    }

    board.push_back(tmpVec);
    tmpVec.clear();

    for (int i{}; i<8; i++) {
        Square *newSquare = new Square;
        newSquare->row = 7;
        newSquare->column = i;

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
    //breaking user input up into two variables, from and to. Then spliting those into
    //characters which are converted to ints and used to navigate the 2d vector board. 
    //f1 and f2 are original coordinates of the piece, and t1 t2 are the goal coordinates
    std::string from, to;
    std::stringstream s(u_input);
    s>>from>>to;

    int f1, f2, t1, t2;
    f1 = static_cast<int>(moveMap[from[0]]);
    //This works because from[x] is an ascii character which can be navigated by adding and
    //subtracting from their ascii code. 
    f2 = 7-(from[1]-49);
    t1 = static_cast<int>(moveMap[to[0]]);
    t2 = 7-(to[1]-49);


    //reflected coordinates to match user input to actual vector coordinates.
    int flippedVer1 = 7-f2;
    int flippedVer2 = 7-t2;

    std::cout << f1;
    //std::cout << f2;
    std::cout << flippedVer1;
    std::cout << std::endl;
    std::cout << t1;
    std::cout << flippedVer2;


    //moving the actual piece at those coordinates;
    //setting position two = to posiition 1; 
    //
    //   - moving piece copies it over destination without removing original
    //   - moving piece out of bounds causes seg fault
    //   - closing the program after moving pieces causes seg fault, probably issue with destructor
    board[t2][t1]->current = board[f2][f1]->current;

    //std::cout << moveMap[from[0]] << from[1] << "-" << moveMap[to[0]] << to[1] << std::endl;
    //std::cout << moveMap[from[0]] << from[1] << "-" << moveMap[to[0]] << to[1] << std::endl;
    //std::cout << moveMap[from[0]] << from[1] << "-" << moveMap[to[0]] << to[1] << std::endl;
    //std::cout << moveMap[from[0]] << from[1] << "-" << moveMap[to[0]] << to[1] << std::endl;
    //std::cout << moveMap[0];
    //<< from[1] << "-" << moveMap[to[0]] << to[1] << std::endl;

    return 0;


}


























