#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include <memory>
#include <map>



class GameBoard {

    public:
        GameBoard();
        ~GameBoard();
        
        void printBoard();
        bool movePiece(std::string);

    private:
        vector<vector<Square*>> board;
        
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
