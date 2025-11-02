#pragma once
#include "../piece.hpp"
class Queen : public Piece
{
public:
    Queen(int x, int y, Colors c) : Piece(x, y, c) {}
    bool move(int xdes,int ydes){
        return ((std::abs(xpos-xdes)==std::abs(ypos-ydes))||((xpos==xdes)||(ypos==ydes)));
    }
    char gettype(){
        return (color == W) ? 'Q' : 'q';
    }
};
