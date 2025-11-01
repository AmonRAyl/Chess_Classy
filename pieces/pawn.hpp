#pragma once
#include "../piece.hpp"
class Pawn : public Piece
{
private:
    bool hasmoved;
public:
    Pawn(int x, int y, Colors c) : Piece(x, y, c) { hasmoved = false; }
    bool move(int xdes, int ydes) {
        if (color == W && (ypos - ydes) == -1 && (xpos - xdes) == 0) {
            return true;
        } else {
            if (color == B && (ypos - ydes) == 1 && (xpos - xdes) == 0)
                return true;
        }
        return false;
    }
    int specialmove(int xdes, int ydes) {
        // Double initial move
        if (!hasmoved) {
            if (color == W && (ypos - ydes) == -2 && (xpos - xdes) == 0) {
                return 1;
            } else {
                if (color == B && (ypos - ydes) == 2 && (xpos - xdes) == 0)
                    return 1;
            }
        }
        // Capture in diagonal //TODO EN PASSAANT KDSFJSKLJFLKJ
        if (color == W && (ypos - ydes) == -1 && std::abs(xpos - xdes) == 1) {
            return 2;
        } else {
            if (color == B && (ypos - ydes) == 1 && std::abs(xpos - xdes) == 1)
                return 2;
        }
        return 0;
    }
    bool gethasmoved(){
        return hasmoved;
    }
    void sethasmoved(bool t){
        hasmoved = t;
    }
    char gettype() {
        return (color == W) ? 'P' : 'p';
    }
};