#include "chessPiece.h"

//vector<std::shared_ptr<MoveData>> ChessPiece::getLegalMoves(vector<vector<ChessPiece*>> board) {
vector<std::shared_ptr<MoveData>> ChessPiece::getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    return legalMoves;
}

//vector<std::shared_ptr<MoveData>> Knight::getLegalMoves(vector<vector<ChessPiece*>> board) {
vector<std::shared_ptr<MoveData>> Knight::getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    return legalMoves;

}



//vector<std::shared_ptr<MoveData>> Pawn::getLegalMoves(vector<vector<ChessPiece*>> board) {
vector<std::shared_ptr<MoveData>> Pawn::getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    int colorSwitch = 1;
    if (color == 'B') {
        colorSwitch*=-1;
    }

    //checks if pawn has moved yet and if squares ahead are empty
    if(row-1*colorSwitch >= 0 && row-1*colorSwitch < board.size()) {
        if(board[row-1*colorSwitch][column]->getName() == 'X') {
            std::shared_ptr<MoveData> newMove(new MoveData);
            newMove->row = row-1*colorSwitch;
            newMove->column = column;
            legalMoves.push_back(newMove);
        }
    }
    if(row-2*colorSwitch >= 0 && row-2*colorSwitch < board.size()) {
        if(board[row-2*colorSwitch][column]->getName() == 'X' && board[row-1*colorSwitch][column]->getName() == 'X' && moved == false) {
            std::shared_ptr<MoveData> newMove(new MoveData);
            newMove->row = row-2*colorSwitch;
            newMove->column = column;
            legalMoves.push_back(newMove);
        }
    }
    //first checks if horizontal moves are in bounds
    //then checks if diagonal squares are occupied by opposite color pieces 
    if (column > 0) {
        if(row-1*colorSwitch >= 0 && row-1*colorSwitch < board.size()) {
            if(board[row-1*colorSwitch][column-1]->getName() != 'X' && board[row-1*colorSwitch][column-1]->getColor() != color) {
                std::shared_ptr<MoveData> newMove(new MoveData);
                newMove->row = row-1*colorSwitch;
                newMove->column = column-1;
                legalMoves.push_back(newMove);
            }
        }
    }
    if (column < board[0].size()-1) {
        if(row-1*colorSwitch >= 0 && row-1*colorSwitch < board.size()) {
            if(board[row-1*colorSwitch][column+1]->getName() != 'X' && board[row-1*colorSwitch][column+1]->getColor() != color) {
                std::shared_ptr<MoveData> newMove(new MoveData);
                newMove->row = row-1*colorSwitch;
                newMove->column = column+1;
                legalMoves.push_back(newMove);
            }
        }
    }
    //for (auto b: legalMoves) {
    //    std::cout << "\n" << b->column << " " << b->row << std::endl;
    //}
    return legalMoves;
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


