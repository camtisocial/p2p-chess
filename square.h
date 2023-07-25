#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include "chessPiece.h"

using std::vector;


class Square {

    public:
        Square();
        ~Square();
        ChessPiece *current;
        int row;
        int column;
    private:

};

#endif
