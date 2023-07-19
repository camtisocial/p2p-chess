#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H


class ChessPiece {
    public:
        ChessPiece();
        ~ChessPiece();
        char color;
        virtual char getName() {
            return 'X';
        }
    private:
};

class Pawn: public ChessPiece {
    public:
        Pawn();
        ~Pawn();
        char color;
        virtual char getName() {
            return 'P';
        }
        
    private:
};

#endif
