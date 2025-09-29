#pragma once
#include <iostream>
#include "cstdlib" //for std::abs
enum Color{
    WHITE, BLACK, NONE
};
class Piece
{
protected:
    int xpos;
    int ypos;
    Color color; // 0-white 1-black
public:
    Piece(int x, int y, Color c) : xpos(x), ypos(y), color(c) {}
    //Setters
    void setX(int x){
        xpos = x;
    }
    void setY(int y){
        ypos = y;
    }
    void setcolor(Color c){
        color = c;
    }
    //Getters
    int getX(){
        return xpos;
    }
    int getY(){
        return ypos;
    }
    Color getcolor(){
        return color;
    }
    //functions
    virtual bool move(int xdes, int ydes) = 0;
    virtual char gettype() = 0;
    virtual int specialmove(int xdes, int ydes) { return 0; }
    virtual void sethasmoved(bool) {}
    virtual bool gethasmoved() { return false; }
    virtual ~Piece() {}
};