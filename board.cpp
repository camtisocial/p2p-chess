#include "board.h"

//Assigning board coordinates to Piece objects
GameBoard::GameBoard() {
    //vector<ChessPiece*> tmpVec;
    vector<std::shared_ptr<ChessPiece>> tmpVec;
    int counter = 0;
///////////////////////////////// SETTING BLACK PIECES\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\;
    for (int i{}; i<8; i++) {
        //ChessPiece *newPiece;
        //This code is repetitive, unfortunately I have not found a way to declare a smart pointer
        //ahead of time, then assign it values as you can with raw pointers
        if (i == 0 || i == 7) {
            //newPiece = new Rook; 
            std::shared_ptr<Rook> newPiece(new Rook);
            newPiece->color = 'B';
            newPiece->setRow(0);
            newPiece->setColumn(i);
            tmpVec.push_back(newPiece);
        }
        else if (i == 1 || i == 6) {
            //newPiece = new Knight; 
            std::shared_ptr<Knight> newPiece(new Knight);
            newPiece->color = 'B';
            newPiece->setRow(0);
            newPiece->setColumn(i);
            tmpVec.push_back(newPiece);
        }
        else if (i == 2 || i == 5) {
            //newPiece = new Bishop; 
            std::shared_ptr<Bishop> newPiece(new Bishop);
            newPiece->color = 'B';
            newPiece->setRow(0);
            newPiece->setColumn(i);
            tmpVec.push_back(newPiece);
        }
        else if (i == 3) {
            //newPiece = new Queen; 
            std::shared_ptr<Queen> newPiece(new Queen);
            newPiece->color = 'B';
            newPiece->setRow(0);
            newPiece->setColumn(i);
            tmpVec.push_back(newPiece);
        } else {
            //newPiece = new King; 
            std::shared_ptr<King> newPiece(new King);
            newPiece->color = 'B';
            newPiece->setRow(0);
            newPiece->setColumn(i);
            tmpVec.push_back(newPiece);
        }

        //newPiece->color = 'B';
        //newPiece->setRow(0);
        //newPiece->setColumn(i);
        //tmpVec.push_back(newPiece);
    }

    board.push_back(tmpVec);
    tmpVec.clear();

    for (int j{}; j<8; j++) {
        //ChessPiece *newPiece = new Pawn; 
        std::shared_ptr<Pawn> newPiece(new Pawn);
        newPiece->setRow(1);
        newPiece->setColumn(j);
        newPiece->color = 'B';
        tmpVec.push_back(newPiece);
    }

    board.push_back(tmpVec);
    tmpVec.clear();

///////////////////////////////// SETTING EMPTY SPACES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\;
    for (int i{}; i<4; i++) {
        for (int j{}; j<8; j++) {
           // ChessPiece *newPiece = new ChessPiece; 
            std::shared_ptr<ChessPiece> newPiece(new ChessPiece);
            newPiece->setRow(i+2);
            newPiece->setColumn(j);

            tmpVec.push_back(newPiece);

            if (j == 7) {
                board.push_back(tmpVec);
                tmpVec.clear();
            }
        }
    }

///////////////////////////////// SETTING WHITE PIECES\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\;
    for (int i{}; i<8; i++) {

        //ChessPiece *newPiece = new Pawn; 
        std::shared_ptr<Pawn> newPiece(new Pawn);
        newPiece->setRow(6);
        newPiece->setColumn(i);
        newPiece->color = 'W';
        tmpVec.push_back(newPiece);
    }

    board.push_back(tmpVec);
    tmpVec.clear();

    for (int i{}; i<8; i++) {
        //ChessPiece *newPiece;
        if (i == 0 || i == 7) {
            //newPiece = new Rook; 
            std::shared_ptr<Rook> newPiece(new Rook);
            newPiece->setRow(7);
            newPiece->setColumn(i);
            newPiece->color = 'W';
            tmpVec.push_back(newPiece);
        }
        else if (i == 1 || i == 6) {
            //newPiece = new Knight; 
            std::shared_ptr<Knight> newPiece(new Knight);
            newPiece->setRow(7);
            newPiece->setColumn(i);
            newPiece->color = 'W';
            tmpVec.push_back(newPiece);
        }
        else if (i == 2 || i == 5) {
            //newPiece = new Bishop; 
            std::shared_ptr<Bishop> newPiece(new Bishop);
            newPiece->setRow(7);
            newPiece->setColumn(i);
            newPiece->color = 'W';
            tmpVec.push_back(newPiece);
        }
        else if (i == 3) {
            //newPiece = new Queen; 
            std::shared_ptr<Queen> newPiece(new Queen);
            newPiece->setRow(7);
            newPiece->setColumn(i);
            newPiece->color = 'W';
            tmpVec.push_back(newPiece);
        } else {
            //newPiece = new King; 
            std::shared_ptr<King> newPiece(new King);
            newPiece->setRow(7);
            newPiece->setColumn(i);
            newPiece->color = 'W';
            tmpVec.push_back(newPiece);
        }

        //newPiece->setRow(7);
        //newPiece->setColumn(i);
        //newPiece->color = 'W';
        //tmpVec.push_back(newPiece);
    }

    board.push_back(tmpVec);
}

GameBoard::~GameBoard() {
    //for (int i{0}; i<8; i++) {
    //    delete (board[0][i]);
    //}
}

void GameBoard::printBoard() {
    for (auto b : board) {
            std::cout << "        ";
        for (int i{}; i<8; i++) {
            char tmp = b[i]->getName();

            if (b[i]->color == 'B') {
                std::cout << "\x1B[92m" << tmp << "\033[0m" <<" ";
            }
            else if(b[i]->color == 'W') {
                std::cout << "\x1B[91m" << tmp << "\033[0m" <<" ";
            } else {
                std::cout << "\x1B[90m" << tmp << "\033[0m" <<" ";

            }
        }
        std::cout << std::endl;
    }
}

/*
bool GameBoard::checkBounds(std::string u_input) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    std::string from, to;
    std::stringstream s(u_input);
    s>>from>>to;

    //converting input to coordinates
    int f1, f2, t1, t2;
    f1 = static_cast<int>(moveMap[from[0]]);
    f2 = 7-(from[1]-49);
    t1 = static_cast<int>(moveMap[to[0]]);
    t2 = 7-(to[1]-49);

}
*/

bool GameBoard::movePiece(std::string u_input) {
    //breaking user input up into two variables, from and to. Then spliting those into
    //characters which are converted to ints and used to navigate the 2d vector board. 
    //f1 and f2 are original coordinates of the piece, and t1 t2 are the goal coordinates
    vector<std::shared_ptr<MoveData>> legalMoves;
    std::string from, to;
    std::stringstream s(u_input);
    s>>from>>to;

    //converting input to coordinates
    int f1, f2, t1, t2;
    f1 = static_cast<int>(moveMap[from[0]]);
    //This works because from[x] is an ascii character which can be navigated by adding and
    //subtracting from their ascii code. 
    f2 = 7-(from[1]-49);
    t1 = static_cast<int>(moveMap[to[0]]);
    t2 = 7-(to[1]-49);

    legalMoves = board[f2][f1]->getLegalMoves(board);
    for (auto b: legalMoves) {
        std::cout << b->column << "-" << b->row << std::endl; 
    }
    

    //tmp piece to replace old pointer to piece with empty square icon
    //ChessPiece *newPiece = new ChessPiece;
    std::shared_ptr<ChessPiece> newPiece(new ChessPiece);

    //setting position 2 = to posiition 1; 
    board[t2][t1] = board[f2][f1];
    board[f2][f1] = newPiece;
    
// issues -/
    //   - moving piece out of bounds causes seg fault
    //   - closing the program after moving pieces causes seg fault, probably issue with destructor

    return 0;


}












