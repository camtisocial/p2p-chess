#ifndef BOARD_H
#define BOARD_H

#include "chessPiece.h"
#include "menu.h"



class GameBoard {

    public:
        GameBoard();
        ~GameBoard();
        
        void printBoardWhite(bool to_play, int turn, std::string whitePieces, std::string blackPieces, std::string boardColor);
        void printBoardBlack(bool to_play, int turn, std::string whitePieces, std::string blackPieces, std::string boardColor);
        bool movePiece(std::string, int);
        char checkForMateOrDraw(int playerTurn);
        int getTerminalWidth();
        void promotePawn(ChessPiece promoter);

    private:
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
