#pragma once
#include "../piece.hpp"
class Pawn : public Piece
{
private:
    bool hasmoved;
public:
    Pawn(int x, int y, Color c) : Piece(x, y, c) { hasmoved = false; }
    bool move(int xdes, int ydes) {
        if (color == WHITE && (ypos - ydes) == -1 && (xpos - xdes) == 0) {
            return true;
        } else {
            if (color == BLACK && (ypos - ydes) == 1 && (xpos - xdes) == 0)
                return true;
        }
        return false;
    }
    int specialmove(int xdes, int ydes) {
        // Double initial move
        if (!hasmoved) {
            if (color == WHITE && (ypos - ydes) == -2 && (xpos - xdes) == 0) {
                return 1;
            } else {
                if (color == BLACK && (ypos - ydes) == 2 && (xpos - xdes) == 0)
                    return 1;
            }
        }
        // Capture in diagonal //TODO EN PASSAANT KDSFJSKLJFLKJ
        if (color == WHITE && (ypos - ydes) == -1 && std::abs(xpos - xdes) == 1) {
            return 2;
        } else {
            if (color == BLACK && (ypos - ydes) == 1 && std::abs(xpos - xdes) == 1)
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
        return (color == WHITE) ? 'P' : 'p';
    }
};