#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "moveData.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

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
        virtual char getName() {return 'X';}

//        vector<MoveData> getLegalMoves(vector<vector<Square*>>);

        ChessPiece(); 
        ~ChessPiece();
    private:
};

class Pawn: public ChessPiece {
    public:
        bool moved = false;
        vector<std::string> legalMoves;

        

        char getName() {return 'P';}
        Pawn();
        ~Pawn();
    private:
};

class Knight: public ChessPiece {
    public:
        char getName() {return 'N';}
        Knight();
        ~Knight();
    private:
};

class Bishop: public ChessPiece {
    public:
        char getName() {return 'B';}
        Bishop();
        ~Bishop();
    private:
};

class Rook: public ChessPiece {
    public:
        char getName() {return 'R';}
        Rook();
        ~Rook();
    private:
};

class Queen: public ChessPiece {
    public:
        char getName() {return 'Q';}
        Queen();
        ~Queen();
    private:
};

class King: public ChessPiece {
    public:
        char getName() {return 'K';}
        King();
        ~King();
    private:
};
#endif
