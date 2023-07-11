#ifndef BOARD_H
#define BOARD_H

#include "square.h"

class GameBoard {

    public:
        GameBoard();
        ~GameBoard();
        
        void printBoard();

    private:
        vector<vector<Square*>> board;
};



#endif //BOARD_H
