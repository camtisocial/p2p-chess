#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include <vector>
#include <string>
using std::vector;

class ChessPiece {
    public:
        int row;
        int column;
        char color;

        void setRow(int input) {row = input;}
        void setColumn(int input) {column = input;}
        int getRow() {return row;}
        int getColumn() {return column;}
        char getName() {return 'X';}

        vector<std::string> legalMoves;

        ChessPiece(); 
        ~ChessPiece();
    private:
};

class Pawn: public ChessPiece {
    public:
        char color;
        int row;
        int column;
        bool moved = false;

        Pawn();
        ~Pawn();
    private:
};

class Knight: public ChessPiece {
    public:
        char color;
        int row;
        int column;
        
        Knight();
        ~Knight();
    private:
};

class Bishop: public ChessPiece {
    public:
        char color;
        int row;
        int column;

        Bishop();
        ~Bishop();
        
    private:
};

class Rook: public ChessPiece {
    public:
        char color;
        int row;
        int column;

        Rook();
        ~Rook();

    private:
};

class Queen: public ChessPiece {
    public:
        char color;
        int row;
        int column;

        Queen();
        ~Queen();
    private:
};

class King: public ChessPiece {
    public:
        char color;
        int row;
        int column;
        
        King();
        ~King();
    private:
};
#endif
