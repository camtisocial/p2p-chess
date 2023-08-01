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
        
        /*
        std::map<std::string, Square*> moveMap =
        {
            {"a1", 0},
            {"a2", 1},
            {"a3", 2},
            {"a4", 3},
            {"a5", 4},
            {"a6", 5},
            {"a7", 6},
            {"a8", 7}
        };
         */
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
