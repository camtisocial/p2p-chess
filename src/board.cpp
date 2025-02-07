#include "board.h"

//Assigning board coordinates to Piece objects
GameBoard::GameBoard() {
    vector<std::shared_ptr<ChessPiece>> tmpVec;
/*@@@@@@@@@@@@@@@** SETTING BLACK PIECES **@@@@@@@@@@@@@@@@*/
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

/*@@@@@@@@@@@@@@@** SETTING EMPTY SPACES **@@@@@@@@@@@@@@@@*/
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

/*@@@@@@@@@@@@@@@** SETTING WHITE PIECES **@@@@@@@@@@@@@@@@*/
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

std::string GameBoard::serializeBoardToFEN(int& toPlay, int& halfMoveClock, int& fullMoveNumber, std::shared_ptr<ChessPiece>& lastMovedPiece) {
    std::string FEN = "";
    int emptyCounter = 0;

    //get board state
    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]->getName() == 'X') {
                emptyCounter++;
            } else {
                if (emptyCounter != 0) {
                    FEN += std::to_string(emptyCounter);
                    emptyCounter = 0;
                }
                if (board[i][j]->color == 'B') {
                    char tmp = board[i][j]->getName();
                    FEN += std::tolower(tmp);
                } else {
                    FEN += board[i][j]->getName();
                }
            }
        }
        if (emptyCounter != 0) {
            FEN += std::to_string(emptyCounter);
            emptyCounter = 0;
        }
        if (i != 7) {
            FEN += "/";
        }
    }

    //get to play
    FEN += (toPlay == 0) ? " b " : " w ";

    //get castling rights
    bool castlingRightsPrinted = false;
    if (!board[7][7]->getMoved() && board[7][7]->getName() == 'R' && !board[7][4]->getMoved() && board[7][4]->getName() == 'K') {
        FEN += "K";
        castlingRightsPrinted = true;
    }
    if (!board[7][0]->getMoved() && board[7][0]->getName() == 'R' && !board[7][4]->getMoved() && board[7][4]->getName() == 'K') {
        FEN += "Q";
        castlingRightsPrinted = true;
    }
    if (!board[0][7]->getMoved() && board[0][7]->getName() == 'R' && !board[0][4]->getMoved() && board[0][4]->getName() == 'K') {
        FEN += "k";
        castlingRightsPrinted = true;
    }
    if (!board[0][0]->getMoved() && board[0][0]->getName() == 'R' && !board[0][4]->getMoved() && board[0][4]->getName() == 'K') {
        FEN += "q";
        castlingRightsPrinted = true;
    }
    if (!castlingRightsPrinted) {
        FEN += "-";
    }

    //get en passant square
    bool enPassant = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]->getLastMoveDouble() && board[i][j]->color == 'W' && board[i][j] == lastMovedPiece) {
                if (j == 0) {
                    if (board[i][j+1]->getName() == 'P' && board[i][j+1]->color == 'B') {
                        FEN += " " + std::string(1, reverseMoveMap[board[i][j]->getColumn()]) + std::to_string(8-(board[i][j]->getRow() + 1));
                        enPassant = true;
                    }
                } else if (j == 7) {
                    if (board[i][j-1]->getName() == 'P' && board[i][j-1]->color == 'B') {
                        FEN += " " + std::string(1, reverseMoveMap[board[i][j]->getColumn()]) + std::to_string(8-(board[i][j]->getRow() + 1));
                        enPassant = true;
                    }
                } else {
                    if (board[i][j+1]->getName() == 'P' && board[i][j+1]->color == 'B') {
                        FEN += " " + std::string(1, reverseMoveMap[board[i][j]->getColumn()]) + std::to_string(8-(board[i][j]->getRow() + 1));
                        enPassant = true;
                    } else if (board[i][j-1]->getName() == 'P' && board[i][j-1]->color == 'B') {
                        FEN += " " + std::string(1, reverseMoveMap[board[i][j]->getColumn()]) + std::to_string(8-(board[i][j]->getRow() + 1));
                        enPassant = true;

                    }
                }


            } else if (board[i][j]->getLastMoveDouble() && board[i][j]->color == 'B' && board[i][j] == lastMovedPiece) {
                if (j == 0) {
                    if (board[i][j+1]->getName() == 'P' && board[i][j+1]->color == 'W') {
                        FEN += " " + std::string(1, reverseMoveMap[board[i][j]->getColumn()]) + std::to_string(8-(board[i][j]->getRow() - 1));
                        enPassant = true;
                    }
                } else if (j == 7) {
                    if (board[i][j-1]->getName() == 'P' && board[i][j-1]->color == 'W') {
                        FEN += " " + std::string(1, reverseMoveMap[board[i][j]->getColumn()]) + std::to_string(8-(board[i][j]->getRow() - 1));
                        enPassant = true;
                    }
                } else {
                    if (board[i][j+1]->getName() == 'P' && board[i][j+1]->color == 'W') {
                        FEN += " " + std::string(1, reverseMoveMap[board[i][j]->getColumn()]) + std::to_string(8-(board[i][j]->getRow() - 1));
                        enPassant = true;
                    } else if (board[i][j-1]->getName() == 'P' && board[i][j-1]->color == 'W') {
                        FEN += " " + std::string(1, reverseMoveMap[board[i][j]->getColumn()]) + std::to_string(8-(board[i][j]->getRow() - 1));
                        enPassant = true;
                    }
                }

            } 
        }
    }
    if (!enPassant) {
        FEN += " -";
    }

    //get half move clock
    FEN += " " + std::to_string(halfMoveClock);

    //get full move number
    FEN += " " + std::to_string(fullMoveNumber);

    return FEN;
}


//ADD GET TERMINAL WIDTH INSIDE LOOP SO IT UPDATES WHEN WINDOW IS RESIZED
int terminalWidth = getTerminalWidth();
void printBoardWhite(std::vector<std::vector<std::shared_ptr<ChessPiece>>> board, bool to_play, float turn, std::string whitePieces, std::string blackPieces,
                     std::string boardColor, std::string altTextColor, std::string lastMovedColor, int labelsOn, std::shared_ptr<ChessPiece> lastMovedPiece,
                     bool& lastMoved, int gameOver, std::string opening, char gameResult, int evalSetting, std::string& currentCentipawnEval, std::vector<std::string>& evalHistory) {

    int openingLen = opening.length();
    int evalLen = currentCentipawnEval.length();
    int openingSpaces = terminalWidth - 18 - openingLen;
    int evalSpaces = terminalWidth - 29 - evalLen;

    std::cout << std::endl;
    if (to_play) {
        std::cout << blackPieces << "   Black " << "\x1B[37m" << "to play" << "\033[0m" << std::string(openingSpaces, ' ') << opening << std::endl;
    } else {
        std::cout << whitePieces << "   White " << "\x1B[37m" << "to play" << "\033[0m" << std::string(openingSpaces, ' ') << opening << std::endl;
    }


    if (evalSetting) {
        //to account for the fact that I am only getting whole turn numbers from fen readings
            int turnIncrement{};
            if (turn == 1 && to_play == 0) {
              turnIncrement = 0;
            } else if (turn == 1 && to_play == 1) {
              turnIncrement = 1;
            }else {
              turnIncrement = ((turn-1)*2)+1;
            }

            int evalAsInt = std::stoi(evalHistory[turnIncrement]);
            float formattedEval = evalAsInt/100.0;

            if (evalAsInt > 0) {
                std::cout << "   Turn: " << static_cast<int>(turn) << std::string(evalSpaces, ' ') << "   Evaluation: +" << whitePieces <<  formattedEval << std::endl;
            } else if (evalAsInt < 0) {
                std::cout << "   Turn: " << static_cast<int>(turn) << std::string(evalSpaces, ' ') << "   Evaluation: " << blackPieces <<  formattedEval << std::endl;
            } else {
                std::cout << "   Turn: " << static_cast<int>(turn) << std::string(evalSpaces, ' ') << "   Evaluation: " << whitePieces <<  formattedEval << std::endl;
            } 
        } else {
            std::cout << "   Turn: " << static_cast<int>(turn) << std::endl;
        }

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;


    for (int i{0}; i<8; i++) {
        std::string rows_ver_a = altTextColor + std::to_string(abs(i - 8)) + "   " + "\033[0m";
        std::string rows_ver_b = altTextColor + std::to_string(abs(i - 8)) +  " " + "\033[0m";

        //check which row label style user wants and print it
        switch (labelsOn) {
            case 1: {
                std::cout << centerText(rows_ver_a, terminalWidth - 6);
                break;
            }
            case 2:
                std::cout << centerText(rows_ver_b, terminalWidth - 2);
                break;
            default:
                std::cout << centerChar(' ', terminalWidth);
                break;
        }

        //print pieces
        for (int j{}; j<8; j++) {
            char tmp = board[i][j]->getName();

            if (board[i][j]->color == 'B') {
                if (board[i][j] == lastMovedPiece && lastMoved) {
                    std::cout << lastMovedColor << tmp << "\033[0m" <<" ";
                } else {
                    std::cout << blackPieces << tmp << "\033[0m" <<" ";
                }
            }
            else if(board[i][j]->color == 'W') {
                if (board[i][j] == lastMovedPiece && lastMoved) {
                    std::cout << lastMovedColor << tmp << "\033[0m" <<" ";
                } else {
                    std::cout << whitePieces << tmp << "\033[0m" <<" ";
                }
            } else {
                std::cout << boardColor << tmp << "\033[0m" <<" ";
            }
        }

        std::cout << std::endl;
    }

    //check which column label style user wants
    std::string A_columnLabels = altTextColor + "a b c d e f g h" + "\033[0m";
    switch (labelsOn) {
        case 1: {
            std::cout << std::endl;
            std::cout << centerText("    ", terminalWidth-15);
            std::cout << A_columnLabels << std::endl;
            break;
        }
        case 2:
            std::cout << centerText("  ", terminalWidth-11);
            std::cout << A_columnLabels << std::endl;
            break;
        default:
    std::cout << std::endl;
            break;
    }

    switch (gameOver) {
        case 1: {
            std::cout << std::endl;
            std::cout << centerText("Press left or right arrow keys to navigate moves. Press enter to return to menu.", getTerminalWidth()) << std::endl;
            break;
        }
        case 2: {
            announceGameResult(gameResult);
            break;
        }
        default: {
            break;
        }
    }

}


void printBoardBlack(std::vector<std::vector<std::shared_ptr<ChessPiece>>> board, bool to_play, float turn, std::string whitePieces, std::string blackPieces,
                     std::string boardColor, std::string altTextColor, std::string lastMovedColor, int labelsOn, std::shared_ptr<ChessPiece> lastMovedPiece, 
                     bool& lastMoved, int gameOver, std::string opening, char gameResult, int evalSetting, std::string& currentCentipawnEval, std::vector<std::string>& evalHistory) {

    int openingLen = opening.length();
    int openingSpaces = terminalWidth - 18 - openingLen;
    int evalSpaces = terminalWidth - 29 - currentCentipawnEval.length();

    std::cout << std::endl;
    if (to_play) {
        std::cout << blackPieces << "   Black " << "\x1B[37m" << "to play" << "\033[0m" << std::string(openingSpaces, ' ') << opening << std::endl;
    } else {
        std::cout << whitePieces << "   White " << "\x1B[37m" << "to play" << "\033[0m" << std::string(openingSpaces, ' ') << opening << std::endl;
    }


    if (evalSetting) {

            int turnIncrement{};
            if (turn == 1 && to_play == 0) {
              turnIncrement = 0;
            } else if (turn == 1 && to_play == 1) {
              turnIncrement = 1;
            }else {
              turnIncrement = ((turn-1)*2)+1;
            }

            int evalAsInt = std::stoi(evalHistory[turnIncrement]);
            float formattedEval = evalAsInt/100.0;

            if (evalAsInt > 0) {
                std::cout << "   Turn: " << static_cast<int>(turn) << std::string(evalSpaces, ' ') << "   Evaluation: +" << whitePieces <<  formattedEval << std::endl;
            } else if (evalAsInt < 0) {
                std::cout << "   Turn: " << static_cast<int>(turn) << std::string(evalSpaces, ' ') << "   Evaluation: " << blackPieces <<  formattedEval << std::endl;
            } else {
                std::cout << "   Turn: " << static_cast<int>(turn) << std::string(evalSpaces, ' ') << "   Evaluation: " << whitePieces <<  formattedEval << std::endl;
            } 
        } else {
            std::cout << "   Turn: " << static_cast<int>(turn) << std::endl;
        }




    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    for (int i{7}; i >= 0; i--) {

            std::string a = altTextColor + std::to_string(abs(i - 8)) + "   " + "\033[0m";
            std::string b = altTextColor + std::to_string(abs(i - 8)) + " " +  "\033[0m";
            switch(labelsOn) {
                case 1: {
                    std::cout << centerText(a, terminalWidth - 6);
                    break;
                }
                case 2:
                    std::cout << centerText(b, terminalWidth - 2);
                    break;
                default:
                    std::cout << centerChar(' ', terminalWidth);
                    break;
            }

        for (int j{7}; j>=0; j--) {
            char tmp = board[i][j]->getName();
            if (board[i][j]->color == 'B') {
                if(board[i][j] == lastMovedPiece && lastMoved) {
                    std::cout << lastMovedColor << tmp << "\033[0m" <<" ";
                } else {
                    std::cout << blackPieces << tmp << "\033[0m" <<" ";
                }
            }
            else if(board[i][j]->color == 'W') {
                if(board[i][j] == lastMovedPiece && lastMoved) {
                    std::cout << lastMovedColor << tmp << "\033[0m" <<" ";
                } else {
                    std::cout << whitePieces << tmp << "\033[0m" <<" ";
                }
            } else {
                std::cout << boardColor << tmp << "\033[0m" <<" ";
            }
        }

        std::cout << std::endl;
    }

        std::string A_columnLabels = altTextColor + "h g f e d c b a" + "\033[0m";
        std::string B_columnLabels = altTextColor + "h g f e d c b a" + "\033[0m";
        switch (labelsOn) {
            case 1: {
                std::cout << std::endl;
                std::cout << centerText("    ", terminalWidth-15);
                std::cout << A_columnLabels << std::endl;
                break;
            }
            case 2:
                std::cout << centerText("  ", terminalWidth-11);
                std::cout << A_columnLabels << std::endl;
                break;
            default:
                std::cout << std::endl;
                break;
        }

        switch (gameOver) {
            case 1: {
                std::cout << std::endl;
                std::cout << centerText("Press left or right arrow keys to navigate moves. Press enter to return to menu.", getTerminalWidth()) << std::endl;
                break;
            }
            case 2: {
                // std::cout << std::endl;
                announceGameResult(gameResult);
                break;
            }
            default: {
                break;
            }
        }

}


void printFromFEN(std::vector<std::vector<std::shared_ptr<ChessPiece>>> board, std::string fen, bool localColor, std::string whitePieces, std::string blackPieces,
                  std::string boardColor, std::string altTextColor, std::string lastMovedColor, int labelsOn, int gameOver, std::string opening, char gameResult,
                  int evalSetting, std::string& currentCentipawnEval, std::vector<std::string>& evalHistory) {

    bool genericBool = false; // this is just so I can pass something into the print function

    //split string into relevant parts
    std::istringstream iss(fen); // stream object 
    std::string boardState, toPlayStr, castlingRights, enPassantSquare, halfMove, fullMoveStr;
    iss >> boardState >> toPlayStr >> castlingRights >> enPassantSquare >> halfMove >> fullMoveStr;

    //formatting turn number
    int toPlay = (toPlayStr == "w") ? 0 : 1;
    int fullMove = std::stoi(fullMoveStr);
    float formattedTurnNum = (toPlayStr == "w" && fen != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") ? fullMove + 0.5 : fullMove; 

    //clear/set board
    board.clear();
    vector<std::shared_ptr<ChessPiece>> row;
    for (char p : boardState) {
        if (p == '/') {
            board.push_back(row);
            row.clear();
        } else if (isdigit(p)) {
            int t = p - '0'; //nifty trick to convert char to int by subtracting ascii value of 0
            for (int i = 0; i < t; i++) {
                std::shared_ptr<ChessPiece> newPiece(new ChessPiece);
                row.push_back(newPiece);
            }
        } else {
            std::shared_ptr<ChessPiece> newPiece;
            switch (std::toupper(p)) {
                case 'P':
                    newPiece = std::make_shared<Pawn>();
                    break;
                case 'R':
                    newPiece = std::make_shared<Rook>();
                    break;
                case 'N':
                    newPiece = std::make_shared<Knight>();
                    break;
                case 'B':
                    newPiece = std::make_shared<Bishop>();
                    break;
                case 'Q':
                    newPiece = std::make_shared<Queen>();
                    break;
                case 'K':
                    newPiece = std::make_shared<King>();
                    break;
            }
            if (std::islower(p)) {
                newPiece->color = 'B';
            } else {
                newPiece->color = 'W';
            }
            row.push_back(newPiece);
        }
    }
    if (!row.empty()) {
        board.push_back(row);
    }
    

    //print board
    if (localColor) {
        printBoardBlack(board, toPlay, formattedTurnNum, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, nullptr, genericBool, gameOver, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory); 
    } else {
        printBoardWhite(board, toPlay, formattedTurnNum, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, nullptr, genericBool, gameOver, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
    }

}

//This should probably be in menu.cpp
void reviewGame(std::vector<std::string> moveHistory, std::vector<std::vector<std::shared_ptr<ChessPiece>>>& board, std::string whitePieces, std::string blackPieces,
                std::string boardColor, std::string altTextColor, std::string lastMovedColor, int labelsOn, std::string opening, char gameResult, int evalSetting,
                std::string& currentCentipawnEval, std::vector<std::string>& evalHistory, bool localColor) {
    int index{};

    while (true) {
        system("clear");
        if (index == 0) {
            printFromFEN(board, moveHistory[index], localColor, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, 1, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
        } else if (index == moveHistory.size() - 1) {
            printFromFEN(board, moveHistory[index], localColor, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, 2, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
        } else {
            printFromFEN(board, moveHistory[index], localColor, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn, 0, opening, gameResult, evalSetting, currentCentipawnEval, evalHistory);
        }
        KeyPress key = getKeyPress();
        if (key == LEFT) {
            if (index > 0) {
                index = (index = index - 1);
            } 
        } else if (key == RIGHT) { 
            if (index+1 < moveHistory.size()) {
                index = (index = index + 1);
            } 
        } else if (key == ENTER) {
            break;
        }
    }   
}

std::string GameBoard::identifyOpening(std::string fen, std::string& opening) {
    std::string boardState, toPlayStr, castlingRights, enPassantSquare, halfMove, fullMoveStr;
    std::istringstream iss(fen);
    iss >> boardState >> toPlayStr >> castlingRights >> enPassantSquare >> halfMove >> fullMoveStr;
    std::string epd = boardState + " " + toPlayStr + " " + castlingRights + " " + enPassantSquare; 

    if (openingsMap.find(epd) != openingsMap.end() && openingsMap[epd] != "") {
        opening = openingsMap[epd];  
    }

    return opening;
}

char GameBoard::checkForMateOrDraw(float playerTurn) {
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
    if (!hasLegalMoves) {
        if (inCheck && playerColor == 'W') {
            return 'B';  //black wins
        } else if (inCheck && playerColor == 'B') {
            return 'W';  //white wins
        } else {
            return 'D';  //draw
        }
    }

    return 'C'; //continue
}


bool GameBoard::movePiece(std::string u_input, int playerTurn, int& halfMoveClock, float& turnNum, std::shared_ptr<ChessPiece>& lastMovedPiece,
                          std::vector<std::string>& moveHistory, std::vector<std::string>& evalHistory, std::string& opening,
                          std::string& stockfishPath, int stockfishDepth, std::string& currentCentipawnEval) {

    std::regex inputPattern("^[a-h][1-8] ?[a-h][1-8]$");


    if (!std::regex_match(u_input, inputPattern)) {
        std::cerr << "Invalid input format. Please use the format 'e2 e4'." << std::endl;
        sleep(1);
        return false;
    }
  
  //add space if user input is in e2e4 format
    if (u_input.size() == 4) {
      u_input.insert(2, " ");
    }

    //bool moveInBounds = false;
    int turnNumInt = static_cast<int>(turnNum);
    char playerColor = (playerTurn == 0) ? 'W' : 'B';
    char opColor = (playerTurn == 0) ? 'B' : 'W';
    bool moveTakesOwnPiece = false;
    bool correctPlayer = false;
    bool moveCompleted = false;
    bool moveCausesCheck = false;
    bool moveIsLegal = false;
    bool moveIsCastling = false;
    vector<std::shared_ptr<MoveData>> legalMoves;
    std::shared_ptr<MoveData> selectedMove = nullptr;


    //Add initial board state to move history
    if (turnNum == 1 && moveHistory.size() == 0) {
        int refInt = 1;
        std::string tmp = serializeBoardToFEN(refInt, halfMoveClock, turnNumInt, lastMovedPiece);
        moveHistory.push_back(tmp);

    }

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
        // std::cout << "Checking move: " << "Row: " << b->row << "  Column: " << b->column <<  "  En Passant: " << b->enPassant << std::endl;
        if (t1==b->column && t2==b->row) {
            moveIsLegal = true;
            selectedMove = b;
            break;
        } else if (b == legalMoves.back() && !moveIsLegal) {
            std::cout << "   Move is not legal: " << u_input << std::endl;
            sleep(1);
            return false;
        }
    }

    //check if move takes own piece
    if (board[t2][t1]->color == playerColor) {
        moveTakesOwnPiece = true;
        std::cout << "   Move is not legal: " << u_input << std::endl;
        sleep(1);
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
        sleep(1);
        return false;
    }


    //Checking if after the move, the player is in check
    if (moveIsLegal && correctPlayer) {
        std::shared_ptr<ChessPiece> targ_temp = board[t2][t1]; // Save the target spot
        std::shared_ptr<ChessPiece> from_temp = board[f2][f1]; // Save the from spot
        board[t2][t1] = board[f2][f1]; // Move the piece
        board[f2][f1] = std::make_shared<ChessPiece>(); // Empty the from spot

        if (selectedMove->enPassant) {
            int capturedPawnRow = (playerTurn == 0) ? t2 + 1 : t2 - 1;
            board[capturedPawnRow][t1] = std::make_shared<ChessPiece>(); // Remove the captured pawn

        }
        
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

        if (selectedMove->enPassant) {
            int capturedPawnRow = (playerTurn == 0) ? t2 + 1 : t2 - 1;
            board[capturedPawnRow][t1] = std::make_shared<Pawn>(); // Replace the captured pawn
            board[capturedPawnRow][t1]->setRow(capturedPawnRow);
            board[capturedPawnRow][t1]->setColumn(t1);
            board[capturedPawnRow][t1]->color = opColor;
            board[capturedPawnRow][t1]->setMoved(true);
            board[capturedPawnRow][t1]->lastMoveDouble = true;
        }
    }


    if (moveCausesCheck) {
        std::cout << "Move causes check" << std::endl;
        sleep(1);  
        return false;
    }
    
    //moving piece pointers.
    if (moveIsLegal && correctPlayer && !moveCausesCheck && !moveTakesOwnPiece) {

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

            if (selectedMove && selectedMove->enPassant) {
                int capturedPawnRow = (playerTurn == 0) ? t2 + 1 : t2 - 1;
                std::shared_ptr<ChessPiece> emptyPiece = std::make_shared<ChessPiece>();
                emptyPiece->setRow(capturedPawnRow);
                emptyPiece->setColumn(t1);
                board[capturedPawnRow][t1] = emptyPiece;
            }
        
        //updating half move counter if piece other than pawn is moved or a piece is captured
            if (board[t2][t1]->getName() != 'X' || board[f2][f1]->getName() == 'P') {
                halfMoveClock = 0;
            } else {
                halfMoveClock++;
            }
            //updating piece coordinates
            //t2 & f2 are rows; t1 & f1 are columns;
            board[f2][f1]->setRow(board[f2][f1]->getRow()+(t2-f2));
            board[f2][f1]->setColumn(board[f2][f1]->getColumn()+(t1-f1));
            board[f2][f1]->setMoved(true);
            board[t2][t1] = board[f2][f1]; //move piece to new location
            std::shared_ptr<ChessPiece> emptyPiece = std::make_shared<ChessPiece>();
            emptyPiece->setRow(f2);
            emptyPiece->setColumn(f1);
            board[f2][f1] = emptyPiece;
            lastMovedPiece = board[t2][t1];

            if (board[t2][t1]->getName() == 'P' && ( t2-f2 == 2 || t2-f2 == -2)) {
                board[t2][t1]->lastMoveDouble = true;
            }


            //checking for promotion
            if (board[t2][t1]->getName() == 'P' && (t2 == 0 || t2 == 7)) {
                std::shared_ptr<ChessPiece> newPiece(new Queen);
                newPiece->setRow(t2);
                newPiece->setColumn(t1);
                newPiece->setMoved(true);
                //why is this member variable the only one without a set() function
                newPiece->color = board[t2][t1]->color;
                board[t2][t1] = newPiece;     
            }

            moveCompleted = true;

            //record board state to moveHistory
            std::string tmpPos = serializeBoardToFEN(playerTurn, halfMoveClock, turnNumInt, lastMovedPiece);
            moveHistory.push_back(tmpPos);

            //record evaluation to evalHistory
            std::string tmpEval{};
            std::thread evalThread(getStockFishEval, tmpPos, stockfishPath,
                                   stockfishDepth, std::ref(evalHistory), std::ref(currentCentipawnEval));
            evalThread.detach();

            //identify opening
            opening = identifyOpening(tmpPos, opening);

        }
        
    return moveCompleted;
}


