#include "chessPiece.h"

vector<std::shared_ptr<MoveData>> ChessPiece::getLegalMoves(vector<vector<ChessPiece*>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    //MoveData 
    return legalMoves;
}
vector<std::shared_ptr<MoveData>> Pawn::getLegalMoves(vector<vector<ChessPiece*>> board) {
    vector<std::shared_ptr<MoveData>> legalMoves;
    if(board[row+1][column]->getName() == 'X') {
        std::shared_ptr<MoveData> newMove(new MoveData);
        newMove->row = row+1;
        newMove->column = column;
        legalMoves.push_back(newMove);
    }
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


