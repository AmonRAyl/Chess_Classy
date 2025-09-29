#pragma once
#include "../piece.hpp"
class King : public Piece
{
private:
    bool hasmoved;
public:
    King(int x, int y, Color c) : Piece(x, y, c) {hasmoved=false;}
    bool move(int xdes,int ydes){
        return (std::abs(xpos-xdes)<=1 && std::abs(ypos-ydes)<=1);
    }
    int specialmove(int xdes,int ydes){
        int offset = (color == WHITE) ? 0 : 6;
        if(!hasmoved){
            //Castle short, right, king
            if (ydes==(0+offset) && xpos-xdes==-2) {
                return 2;
            }
            //Castle long, left, queen
            if (ydes==(0+offset) && xpos-xdes==2) {
                return 1;
            }
        }
        return 0;
    }
    char gettype(){
        return (color == WHITE) ? 'K' : 'k';
    }
};