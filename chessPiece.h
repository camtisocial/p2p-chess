#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H


//CREATE .CPP IMPLEMENTATION
class ChessPiece {
    public:
        ChessPiece();
        ~ChessPiece();
        char name = 'X';
    private:
};

class Pawn: public ChessPiece {
    public:
        Pawn();
        ~Pawn();
        char name = 'P';
        
    private:
};

#endif
