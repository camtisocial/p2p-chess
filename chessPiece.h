#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H


class ChessPiece {
    public:
        char color;
        virtual char getName() {
            return 'X';
        }

        ChessPiece(); 
        ~ChessPiece();
    private:
};

class Pawn: public ChessPiece {
    public:
        char color;
        virtual char getName() {
            return 'P';
        }
        
        Pawn();
        ~Pawn();
    private:
};

class Knight: public ChessPiece {
    public:
        char color;
        virtual char getName() {
            return 'N';
        }
        
        Knight();
        ~Knight();
    private:
};

class Bishop: public ChessPiece {
    public:
        char color;
        virtual char getName() {
            return 'B';
        }

        Bishop();
        ~Bishop();
        
    private:
};

class Rook: public ChessPiece {
    public:
        char color;
        virtual char getName() {
            return 'R';
        }
        
        Rook();
        ~Rook();

    private:
};

class Queen: public ChessPiece {
    public:
        char color;
        virtual char getName() {
            return 'Q';
        }
        
        Queen();
        ~Queen();
    private:
};

class King: public ChessPiece {
    public:
        char color;
        virtual char getName() {
            return 'K';
        }
        
        King();
        ~King();
    private:
};
#endif
