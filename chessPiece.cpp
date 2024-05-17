#include "chessPiece.h"


// vector<std::shared_ptr<MoveData>> ChessPiece::getLegalMoves(vector<vector<ChessPiece*>> board) {
vector<std::shared_ptr<MoveData>> ChessPiece::getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    return legalMoves;
}

//vector<std::shared_ptr<MoveData>> Knight::getLegalMoves(vector<vector<ChessPiece*>> board) {
vector<std::shared_ptr<MoveData>> Knight::getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    int moves[2] = {-1, 1};

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            //get all possible variations of +2, -2, +1, -1 from original position
            int newRow1 = row + 2 * moves[i];
            int newColumn1 = column + moves[j];
            int newRow2 = row + moves[i];
            int newColumn2 = column + 2 * moves[j];

            if (newRow1 >= 0 && newRow1 < 7 && newColumn1 >= 0 && newColumn1 < 7 && board[newRow1][newColumn1]->getColor() != color) {
                std::shared_ptr<MoveData> newMove(new MoveData);
                newMove->row = 7-(8 - 1 - newRow1);
                newMove->column = newColumn1;
                legalMoves.push_back(newMove);
            }

            if (newRow2 >= 0 && newRow2 < 7 && newColumn2 >= 0 && newColumn2 < 7 && board[newRow2][newColumn2]->getColor() != color) {
                std::shared_ptr<MoveData> newMove(new MoveData);
                newMove->row = 7-(8 - 1 - newRow2);
                newMove->column = newColumn2;
                legalMoves.push_back(newMove);
            }
        }
    }

    return legalMoves;
}

vector<std::shared_ptr<MoveData>> Pawn::getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    bool rowCheck = (row >= 1 || row <= 7);
    int colorSwitch = 1;
    if (color == 'B') {
        colorSwitch*=-1;
    }

    //checks if pawn has moved yet and if squares ahead are empty
    if (rowCheck) {
        if(row-1*colorSwitch >= 0 && row-1*colorSwitch < board.size()) {
            if(board[row-1*colorSwitch][column]->getName() == 'X') {
                std::shared_ptr<MoveData> newMove(new MoveData);
                newMove->row = row-1*colorSwitch;
                newMove->column = column;
                legalMoves.push_back(newMove);
            }
        }
    }
    if(moved == false) {
        if(board[row-2*colorSwitch][column]->getName() == 'X' && board[row-1*colorSwitch][column]->getName() == 'X') {
            std::shared_ptr<MoveData> newMove(new MoveData);
            newMove->row = row-2*colorSwitch;
            newMove->column = column;
            legalMoves.push_back(newMove);
        }
    }

    //first checks if horizontal moves are in bounds
    //then checks if diagonal squares are occupied by opposite color pieces 
    if (column > 0 && rowCheck) {
            if(board[row-1*colorSwitch][column-1]->getName() != 'X' && board[row-1*colorSwitch][column-1]->getColor() != color) {
                std::shared_ptr<MoveData> newMove(new MoveData);
                newMove->row = row-1*colorSwitch;
                newMove->column = column-1;
                legalMoves.push_back(newMove);
            }
        }
    if (column < board[0].size()-1) {
            if(board[row-1*colorSwitch][column+1]->getName() != 'X' && board[row-1*colorSwitch][column+1]->getColor() != color) {
                std::shared_ptr<MoveData> newMove(new MoveData);
                newMove->row = row-1*colorSwitch;
                newMove->column = column+1;
                legalMoves.push_back(newMove);
            }
        }
    return legalMoves;
}

vector<std::shared_ptr<MoveData>> Bishop::getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    int diag_counter = 1; //This tells the loop to keep repeating until it diagonally runs into a piece or edge of board
    bool stopFound = false;
    char op_color;
    if (color == 'W') {
        op_color = 'B';
    }
    else {
        op_color = 'W';
    }
    while (row + diag_counter <= 7 && column + diag_counter <= 7 && !stopFound) {
        if (board[row+diag_counter][column+diag_counter]->getColor() != color) {
            std::shared_ptr<MoveData> newMove(new MoveData);
            newMove->row = row+diag_counter;
            newMove->column = column+diag_counter;
            legalMoves.push_back(newMove);
        }
        if (board[row+diag_counter][column+diag_counter]->getColor() == op_color) {
            stopFound = true;
        }
        diag_counter++;
    }
    diag_counter = 1;
    stopFound = false;
    while (row + diag_counter <= 7 && column - diag_counter >= 0 && !stopFound) {
        if (board[row+diag_counter][column-diag_counter]->getColor() != color) {
            std::shared_ptr<MoveData> newMove(new MoveData);
            newMove->row = row+diag_counter;
            newMove->column = column-diag_counter;
            legalMoves.push_back(newMove);
        }
        if (board[row+diag_counter][column-diag_counter]->getColor() == op_color) {
            stopFound = true;
        }
        diag_counter++;
    }
    diag_counter = 1;
    stopFound = false;
    while (row - diag_counter >= 0 && column + diag_counter <= 7 && !stopFound) {
        if (board[row - diag_counter][column + diag_counter]->getColor() != color) {
            std::shared_ptr<MoveData> newMove(new MoveData);
            newMove->row = row - diag_counter;
            newMove->column = column + diag_counter;
            legalMoves.push_back(newMove);
        }
        if (board[row - diag_counter][column + diag_counter]->getColor() == op_color) {
            stopFound = true;
        }
        diag_counter++;
    }
    diag_counter = 1;
    stopFound = false;
    while (row - diag_counter >= 0 && column - diag_counter >= 0 && !stopFound) {
        if (board[row-diag_counter][column-diag_counter]->getColor() != color) {
            std::shared_ptr<MoveData> newMove(new MoveData);
            newMove->row = row-diag_counter;
            newMove->column = column-diag_counter;
            legalMoves.push_back(newMove);
        }
        if (board[row-diag_counter][column-diag_counter]->getColor() == op_color) {
            stopFound = true;
        }
        diag_counter++;
    }
    return legalMoves;
}

vector<std::shared_ptr<MoveData>> Rook::getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    bool stopFound = false;
    char op_color;
    if (color == 'W') {
        op_color = 'B';
    }
    else {
        op_color = 'W';
    }
    int square_counter = 1;
    stopFound = false;

    while (row - square_counter >= 0 && !stopFound) {
        if (board[row-square_counter][column]->getColor() != color) {
            std::shared_ptr<MoveData> newMove(new MoveData);
            newMove->row = row-square_counter;
            newMove->column = column;
            legalMoves.push_back(newMove);
        }
        if (board[row-square_counter][column]->getColor() == op_color) {
            stopFound = true;
        }
        square_counter++;
    }

 
}









ChessPiece::ChessPiece() {
    color = 'E';
}
ChessPiece::~ChessPiece() {
}

Pawn::Pawn() {
    color = 'E';
}
Pawn::~Pawn() {
}


Knight::Knight() {
    color = 'E';
}
Knight::~Knight() {
}


Bishop::Bishop() {
    color = 'E';
}
Bishop::~Bishop() {
}

Rook::Rook() {
    color = 'E';
}
Rook::~Rook() {
}


Queen::Queen() {
    color = 'E';
}
Queen::~Queen() {
}


King::King() {
    color = 'E';
}
King::~King() {
}


