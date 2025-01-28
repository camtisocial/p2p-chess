#ifndef BOARD_H
#define BOARD_H

#include "chessPiece.h"
#include "menu.h"
#include <cctype>


class GameBoard {

    public:
        GameBoard();
        ~GameBoard();
        
        void promotePawn(ChessPiece promoter);
        std::string serializeBoardToFEN(int& toPlay, int& halfMoveClock, int& fullMoveNumber, std::shared_ptr<ChessPiece>& lastMovedPiece);
        bool movePiece(std::string move, int playerTurn, int& halfMoveClock, float& turnNum, std::shared_ptr<ChessPiece>& lastMovedPiece, std::vector<std::string>& moveHistory);
        char checkForMateOrDraw(float playerTurn);
        int getTerminalWidth();
        std::vector<std::vector<std::shared_ptr<ChessPiece>>>& getBoard() {return board;};


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

        std::map<int, char> reverseMoveMap =
        {
            {0, 'a'},
            {1, 'b'},
            {2, 'c'},
            {3, 'd'},
            {4, 'e'},
            {5, 'f'},
            {6, 'g'},
            {7, 'h'}
        };

};


//printing functions
void printBoardWhite(std::vector<std::vector<std::shared_ptr<ChessPiece>>> board, bool to_play, float turn, std::string whitePieces, std::string blackPieces, std::string boardColor, std::string altTextColor, std::string lastMovedColor, int labelsOn, std::shared_ptr<ChessPiece> lastMovedPiece, bool& lastMoved, bool gameOver);
void printBoardBlack(std::vector<std::vector<std::shared_ptr<ChessPiece>>> board, bool to_play, float turn, std::string whitePieces, std::string blackPieces, std::string boardColor, std::string altTextColor, std::string lastMovedColor, int labelsOn, std::shared_ptr<ChessPiece> lastMovedPiece, bool& lastMoved, bool gameOver);
void reviewGame(std::vector<std::string> moveHistory, std::vector<std::vector<std::shared_ptr<ChessPiece>>>& board, std::string whitePieces, std::string blackPieces, std::string boardColor, std::string altTextColor, std::string lastMovedColor, int labelsOn);
void printFromFEN(std::vector<std::vector<std::shared_ptr<ChessPiece>>> board, std::string fen, bool localColor, std::string whitePieces, std::string blackPieces, std::string boardColor, std::string altTextColor, std::string lastMovedColor, int labelsOn, bool gameOver);

#endif //BOARD_H
