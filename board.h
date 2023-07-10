#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>


class GameBoard {

    public:
        GameBoard();
        ~GameBoard();
        

    private:
        vector<vector<chessPiece>> board
        {
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
        }
};



#endif BOARD_H
