#ifndef BOARD_H
#define BOARD_H

#include "chessPiece.h"
#include "menu.h"



class GameBoard {

    public:
        GameBoard();
        ~GameBoard();
        
        void printBoardWhite(bool to_play, float turn, std::string whitePieces, std::string blackPieces, std::string boardColor, std::string altTextColor, int labelsOn);
        void printBoardBlack(bool to_play, float turn, std::string whitePieces, std::string blackPieces, std::string boardColor, std::string altTextColor, int labelsOn);
        bool movePiece(std::string, float playerTurn);
        char checkForMateOrDraw(float playerTurn);
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
