#ifndef BOARD_H
#define BOARD_H

//#include "square.h"
#include "chessPiece.h"



class GameBoard {

    public:
        GameBoard();
        ~GameBoard();
        
        void printBoard();
        bool movePiece(std::string, int);
        void promotePawn(ChessPiece promoter);

        //bool checkBounds(std::string);


    private:
        //vector<vector<ChessPiece*>> board;
        vector<vector<std::shared_ptr<ChessPiece>>> board;
        
        std::map<char, int> moveMap =
        {
            {'a', 0},
            {'b', 1},
            {'c', 2},
            {'d', 3},
            {'e', 4},
            {'f', 5},
            {'g', 6},
            {'h', 7}
        };

};



#endif //BOARD_H
