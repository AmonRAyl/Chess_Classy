#pragma once
#include "../piece.hpp"
class Bishop : public Piece
{
public:
    Bishop(int x, int y, Color c) : Piece(x, y, c) {}
    bool move(int xdes,int ydes){
        return (std::abs(xpos-xdes)==std::abs(ypos-ydes));
    }
    char gettype(){
        return (color == WHITE) ? 'B' : 'b';
    }
};
