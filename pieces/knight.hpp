#pragma once
#include "../piece.hpp"
class Knight : public Piece
{
public:
    Knight(int x, int y, Color c) : Piece(x, y, c) {}
    bool move(int xdes,int ydes){
        return ((std::abs(xpos-xdes) + std::abs(ypos-ydes)) == 3);
    }
    char gettype(){
        return (color == WHITE) ? 'N' : 'n';
    }
};
