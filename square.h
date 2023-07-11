#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <vector>
#include <string>
#include "chessPiece.h"
using std::vector;


class Square {

    public:
        Square();
        ~Square();
        int color;
        ChessPiece current;

    private:

};

#endif
