#include "board.h"

//Assigning board coordinates to Piece objects
GameBoard::GameBoard() {
    //vector<ChessPiece*> tmpVec;
    vector<std::shared_ptr<ChessPiece>> tmpVec;
    int counter = 0;
///////////////////////////////// SETTING BLACK PIECES\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\;
    for (int i{}; i<8; i++) {
        std::shared_ptr<ChessPiece> newPiece;

        if (i == 0 || i == 7) {
            newPiece = std::make_shared<Rook>();
        }
        else if (i == 1 || i == 6) {
            newPiece = std::make_shared<Knight>();
        }
        else if (i == 2 || i == 5) {
            newPiece = std::make_shared<Bishop>();
        }
        else if (i == 3) {
            newPiece = std::make_shared<Queen>();
        }
        else {
            newPiece = std::make_shared<King>();
        }

        newPiece->setRow(0);
        newPiece->setColumn(i);
        newPiece->color = 'B';

        tmpVec.push_back(newPiece);

    }


    board.push_back(tmpVec);
    tmpVec.clear();

    for (int j{}; j<8; j++) {
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

        std::shared_ptr<Pawn> newPiece(new Pawn);
        newPiece->setRow(6);
        newPiece->setColumn(i);
        newPiece->color = 'W';
        tmpVec.push_back(newPiece);
    }

    board.push_back(tmpVec);
    tmpVec.clear();

    for (int i{}; i<8; i++) {
        std::shared_ptr<ChessPiece> newPiece;

        if (i == 0 || i == 7) {
            newPiece = std::make_shared<Rook>();
        }
        else if (i == 1 || i == 6) {
            newPiece = std::make_shared<Knight>();
        }
        else if (i == 2 || i == 5) {
            newPiece = std::make_shared<Bishop>();
        }
        else if (i == 3) {
            newPiece = std::make_shared<Queen>();
        } else  {
            newPiece = std::make_shared<King>();
        }

        newPiece->setRow(7);
        newPiece->setColumn(i);
        newPiece->color = 'W';

        tmpVec.push_back(newPiece);

    }

    board.push_back(tmpVec);
}

GameBoard::~GameBoard() {
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

bool GameBoard::movePiece(std::string u_input, int playerTurn) {
    //breaking user input up into two variables, from and to. Then spliting those into
    //characters which are converted to ints and used to navigate the 2d vector board. 
    //f1 and f2 are original coordinates of the piece, and t1 t2 are the goal coordinates
    //bool moveInBounds = false;
    bool correctPlayer = false;
    bool moveCompleted = false;
    bool moveCausesCheck = false;
    bool moveIsLegal = false;
    vector<std::shared_ptr<MoveData>> legalMoves;
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

    //checking if move is legal based on bounds. 
    //if (f1<=7 && f1>=0 &&
    //    f2<=7 && f2>=0 &&
    //    t1<=7 && t1>=0 &&
    //     t2<=7 && t2>=0) 
    //     {moveInBounds = true;}

    //checking if move is legal based on piece
    legalMoves = board[f2][f1]->getLegalMoves(board);
    for (auto b: legalMoves) {
        if (t1==b->column && t2==b->row) {
            moveIsLegal = true;
        }
    }

    //checking if move is legal based on whose turn it is
    if (playerTurn == 0 && board[f2][f1]->color == 'W') {
        correctPlayer = true;
    } else if (playerTurn == 1 && board[f2][f1]->color == 'B') {
        correctPlayer = true;
    }

    for (auto b: legalMoves) {
        std::cout << b->column << "-" << b->row << std::endl; 
    }

    //moving piece pointers.
    if (moveIsLegal && correctPlayer) {
        std::shared_ptr<ChessPiece> newPiece(new ChessPiece);
       
        //updating piece coordinates
        //t2 & f2 are rows; t1 & f1 are columns;
        board[f2][f1]->setRow(board[f2][f1]->getRow()+(t2-f2));
        board[f2][f1]->setColumn(board[f2][f1]->getColumn()+(t1-f1));
        board[f2][f1]->setMoved(true);
        board[t2][t1] = board[f2][f1];
        board[f2][f1] = newPiece;

        //checking for promotion
        if (board[t2][t1]->getName() == 'P' && (t2 == 0 || t2 == 7)) {
            std::cout << "promotion occurs" << std::endl;  
            std::shared_ptr<ChessPiece> newPiece(new Queen);
            newPiece->setRow(t2);
            newPiece->setColumn(t1);
            newPiece->setMoved(true);
            newPiece->color = board[t2][t1]->color;
            board[t2][t1] = newPiece;     
        }
        moveCompleted = true;
        
    } else {
        std::cout << "\tInvalid move, try again" << std::endl;
    }

    
// issues -/
//          -Pawn causes seg fault when reacing 8th rank, need to add bounds checking to getLegalMoves method


    return moveCompleted;
}


