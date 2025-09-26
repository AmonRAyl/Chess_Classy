#pragma once
#include "../piece.hpp"
class King : public Piece
{
private:
    bool hasmoved;
public:
    King(int x, int y, Color c) : Piece(x, y, c) {hasmoved=false;}
    bool move(int xdes,int ydes){
        return (((std::abs(xpos-xdes))==(std::abs(ypos-ydes)))==1);
    }
    bool specialmove(int xdes,int ydes){
        //TODO
        return false;
    }
    char gettype(){
        return (color == WHITE) ? 'K' : 'k';
    }
};
