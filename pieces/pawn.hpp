#pragma once
#include "../piece.hpp"
class Pawn : public Piece
{
private:
    bool hasmoved;
public:
    Pawn(int x, int y, Color c) : Piece(x, y, c) {hasmoved = false;}
    bool move(int xdes,int ydes){
        if (color==0 && (ypos-ydes)==-1){
            return true;
        } else {
            if (color==1 && (ypos-ydes)==1)
                return true;            
        }
        return false;
    }
    bool specialmove(){
        //TODO
        return false;
    }
    char gettype(){
        return (color == WHITE) ? 'P' : 'p';
    }
};