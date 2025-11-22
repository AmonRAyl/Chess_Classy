#pragma once
#include "../piece.hpp"
class Empty : public Piece{
public:
    Empty(int x, int y, Colors c) : Piece(x, y, c) {}
    bool move(int xdes,int ydes){
        return false;
    }
    char gettype(){
        return '-';
    }
}; 
