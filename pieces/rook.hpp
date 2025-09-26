#pragma once
#include "../piece.hpp"
class Rook : public Piece
{
private:
    bool hasmoved;
public:
    Rook(int x, int y, Color c) : Piece(x, y, c) {hasmoved=false;}
    bool move(int xdes,int ydes){
        return ((xpos==xdes)||(ypos==ydes));
    }
    char gettype(){
        return (color == WHITE) ? 'R' : 'r';
    }
};