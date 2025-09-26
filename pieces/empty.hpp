#pragma once
#include "../piece.hpp"
class Empty : public Piece{ //TODO: Cuidador con el nombre
public:
    Empty(int x, int y, Color c) : Piece(x, y, c) {}
    bool move(int xdes,int ydes){
        return false;
    }
    char gettype(){
        return '-';
    }
}; 
