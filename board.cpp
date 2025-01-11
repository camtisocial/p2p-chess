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

//ADD GET TERMINAL WIDTH INSIDE LOOP SO IT UPDATES WHEN WINDOW IS RESIZED
int terminalWidth = getTerminalWidth();
void GameBoard::printBoardWhite(bool to_play, int turn) {
    std::cout << std::endl;
    if (to_play) {
        std::cout << "\x1B[1;91m" << "   Black " << "\x1B[1;92m" << "to play" << "\033[0m" << std::endl;
    } else {
        std::cout << "\x1B[1;92m" << "   White " << "\x1B[1;92m" << "to play" << "\033[0m" <<std::endl;
    }
    std::cout << "   Turn: " << turn << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    for (auto b : board) {
            std::cout << centerChar(' ', terminalWidth);
        for (int i{}; i<8; i++) {
            char tmp = b[i]->getName();

            if (b[i]->color == 'B') {
                std::cout << "\x1B[1;91m" << tmp << "\033[0m" <<" ";
            }
            else if(b[i]->color == 'W') {
                std::cout << "\x1B[1;92m" << tmp << "\033[0m" <<" ";
            } else {
                std::cout << "\x1B[1;90m" << tmp << "\033[0m" <<" ";

            }
        }
        std::cout << std::endl;
    }
}

void GameBoard::printBoardBlack(bool to_play, int turn) {
    std::cout << std::endl;
    if (to_play) {
        std::cout << "\x1B[1;91m" << "   Black " << "\x1B[1;92m" << "to play" << "\033[0m" << std::endl;
    } else {
        std::cout << "\x1B[1;92m" << "   White " << "\x1B[1;92m" << "to play" << "\033[0m" <<std::endl;
    }
    std::cout << "   Turn: " << turn << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    for (int i{7}; i >= 0; i--) {
            std::cout << centerChar(' ', terminalWidth);
        for (int j{7}; j>=0; j--) {
            char tmp = board[i][j]->getName();

            if (board[i][j]->color == 'B') {
                std::cout << "\x1B[1;91m" << tmp << "\033[0m" <<" ";
            }
            else if(board[i][j]->color == 'W') {
                std::cout << "\x1B[1;92m" << tmp << "\033[0m" <<" ";
            } else {
                std::cout << "\x1B[1;90m" << tmp << "\033[0m" <<" ";

            }
        }
        std::cout << std::endl;
    }
}


void GameBoard::saveBoardState(int turnNum, int playerTurn, nlohmann::json &jsonObject) {
    jsonObject["turnNum"] = turnNum;
    jsonObject["playerTurn"] = playerTurn;
    //BUG WHERE WHEN A PIECE MOVES, THE NEWLY EMPTY SPACE HAS GARBAGE DATA FOR ITS ROW AND COLUMN VALUES

    for (const auto& row : board) {
        for (const auto& piece : row) {
            if (piece) {
                jsonObject["board"].push_back({
                    {"type", std::string(1, piece->getName())},
                    {"row", piece->getRow()},
                    {"column", piece->getColumn()},
                    {"color", std::string(1, piece->getColor())},
                });
            } else {
                jsonObject["board"].push_back(nullptr);
            }
        }
    }
}


char GameBoard::checkForMateOrDraw(int playerTurn) {
    char playerColor = (playerTurn == 0) ? 'B' : 'W';
    char opColor = (playerTurn == 0) ? 'W' : 'B';

    // Check if the player is in check
    bool inCheck = false;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j]->color == opColor) {
                auto legalMoves = board[i][j]->getLegalMoves(board);
                for (auto move : legalMoves) {
                    if (board[move->row][move->column]->getName() == 'K' && board[move->row][move->column]->color == playerColor) {
                        inCheck = true;
                        break;
                    }
                }
            }
        }
    }

    // Check if the player has any legal moves
    bool hasLegalMoves = false;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j]->color == playerColor) {
                auto legalMoves = board[i][j]->getLegalMoves(board);
                for (auto move : legalMoves) {
                    // Simulate the move
                    std::shared_ptr<ChessPiece> targ_temp = board[move->row][move->column];
                    std::shared_ptr<ChessPiece> from_temp = board[i][j];
                    board[move->row][move->column] = board[i][j];
                    board[i][j] = std::make_shared<ChessPiece>();

                    // Check if the player is still in check
                    bool stillInCheck = false;
                    for (int x = 0; x < 8; ++x) {
                        for (int y = 0; y < 8; ++y) {
                            if (board[x][y]->color == opColor) {
                                auto opponentMoves = board[x][y]->getLegalMoves(board);
                                for (auto opMove : opponentMoves) {
                                    if (board[opMove->row][opMove->column]->getName() == 'K' && board[opMove->row][opMove->column]->color == playerColor) {
                                        stillInCheck = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    // reset to actual state of the board
                    board[i][j] = from_temp;
                    board[move->row][move->column] = targ_temp;

                    if (!stillInCheck) {
                        hasLegalMoves = true;
                        break;
                    }
                }
            }
        }
    }

//return result
//TODO currently, this return value is not used to determine if black or white won, should be changed in main or this code should be removed
    if (!hasLegalMoves) {
        if (inCheck && playerColor == 'W') {
            return 'W';  //black wins
        } else if (inCheck && playerColor == 'B') {
            return 'B';  //white wins
        } else {
            return 'D';  //draw
        }
    }

    return 'C'; //continue
}


bool GameBoard::movePiece(std::string u_input, int playerTurn) {

    std::regex inputPattern("^[a-h][1-8] [a-h][1-8]$");

    if (!std::regex_match(u_input, inputPattern)) {
        std::cerr << "Invalid input format. Please use the format 'e2 e4'." << std::endl;
        return false;
    }

    //bool moveInBounds = false;
    char playerColor = (playerTurn == 0) ? 'W' : 'B';
    char opColor = (playerTurn == 0) ? 'B' : 'W';
    bool correctPlayer = false;
    bool moveCompleted = false;
    bool moveCausesCheck = false;
    bool moveIsLegal = false;
    bool moveIsCastling = false;
    vector<std::shared_ptr<MoveData>> legalMoves;

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


    //checking if move is legal based on piece
    legalMoves = board[f2][f1]->getLegalMoves(board);
    for (auto b: legalMoves) {
        if (t1==b->column && t2==b->row) {
            moveIsLegal = true;
            break;
        } else if (b == legalMoves.back() && !moveIsLegal) {
            std::cout << "Move is not legal" << std::endl;
        }
    }

    //check if move is castling
    if (board[f2][f1]->getName() == 'K' && abs(t1 - f1) == 2) { 
        moveIsCastling = true;
    }

    //checking if move is legal based on whose turn it is
    if (playerTurn == 0 && board[f2][f1]->color == playerColor) {
        correctPlayer = true;
    } else if (playerTurn == 1 && board[f2][f1]->color == playerColor) {
        correctPlayer = true;
    } else {
        std::cout << "It is not your turn" << std::endl;
    }


    //Checking if after the move, the player is in check
    if (moveIsLegal && correctPlayer) {
        std::shared_ptr<ChessPiece> targ_temp = board[t2][t1]; // Save the target spot
        std::shared_ptr<ChessPiece> from_temp = board[f2][f1]; // Save the from spot
        board[t2][t1] = board[f2][f1]; // Move the piece
        board[f2][f1] = std::make_shared<ChessPiece>(); // Empty the from spot
        
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j]->color == opColor) {
                    auto legalMoves = board[i][j]->getLegalMoves(board); //type is vector<shared_ptr<MoveData>>

                    // If the king of the current player is in the legal moves, return true
                    for (auto move : legalMoves) {
                        if (board[move->row][move->column]->getName() == 'K' && board[move->row][move->column]->color == playerColor) {
                            moveCausesCheck = true;
                        }
                    }
                }
            }
        }
        board[f2][f1] = from_temp; // Reset the from spot
        board[t2][t1] = targ_temp; // Reset the target spot
    }


    if (moveCausesCheck) {
        std::cout << "Move causes check" << std::endl;
    }
    
    //moving piece pointers.
    if (moveIsLegal && correctPlayer && !moveCausesCheck) {

        //moving rook for castling
        if(moveIsCastling) {
            // Kingside castling
            if (t1 > f1) { 
                board[f2][f1 + 1] = board[f2][7]; // Move rook to its new position (1 to the right of the king)
                board[f2][7] = std::make_shared<ChessPiece>(); // Clear old rook position
                board[f2][f1 + 1]->setRow(f2);
                board[f2][f1 + 1]->setColumn(f1 + 1);
                board[f2][f1 + 1]->setMoved(true);
                moveCompleted = true;

            // Queenside castling
            } else { 
                board[f2][f1 - 1] = board[f2][0]; // Move rook to its new position (1 to the left of the king)
                board[f2][0] = std::make_shared<ChessPiece>(); // Clear old rook position
                board[f2][f1 - 1]->setRow(f2);
                board[f2][f1 - 1]->setColumn(f1 - 1);
                board[f2][f1 - 1]->setMoved(true);
                moveCompleted = true;
            }
        } 
            //updating piece coordinates
            //t2 & f2 are rows; t1 & f1 are columns;
            board[f2][f1]->setRow(board[f2][f1]->getRow()+(t2-f2));
            board[f2][f1]->setColumn(board[f2][f1]->getColumn()+(t1-f1));
            board[f2][f1]->setMoved(true);
            board[t2][t1] = board[f2][f1];
            // board[f2][f1] = std::make_shared<ChessPiece>();
            std::shared_ptr<ChessPiece> emptyPiece = std::make_shared<ChessPiece>();
            emptyPiece->setRow(f2);
            emptyPiece->setColumn(f1);
            board[f2][f1] = emptyPiece;


            //checking for promotion
            if (board[t2][t1]->getName() == 'P' && (t2 == 0 || t2 == 7)) {
                // std::cout << "promotion occurs" << std::endl;  
                std::shared_ptr<ChessPiece> newPiece(new Queen);
                newPiece->setRow(t2);
                newPiece->setColumn(t1);
                newPiece->setMoved(true);
                newPiece->color = board[t2][t1]->color;
                board[t2][t1] = newPiece;     
            }
            moveCompleted = true;
        }
        
    return moveCompleted;
}


