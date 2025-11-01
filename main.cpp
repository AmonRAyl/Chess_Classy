#include "handler.hpp"
#include <string.h>
#include "raylib.h"

int main(){
    //Board and Pieces initialization
    Handler H;
    //Game loop
    bool win = false;
    bool tie = false;
    int xcoord1,ycoord1,xcoord2,ycoord2;

    char color[6]; 

    while (win == false && tie==false) {
        H.printboard();
        
        //TODO: Hide menu
        strcpy(color, (H.getcurrentcolor() == W) ? "WHITE" : "BLACK");
        std::cout << color <<" to play" << std::endl;
        std::cout << "What piece do you want to move? xy, numbers 1 to 8" << std::endl;
        std::cin >> xcoord1 >> ycoord1; //TODO handle stupidity
        std::cout << "Where do you want to move your " << H.getBoard()[xcoord1-1][ycoord1-1]->gettype() << "? xy, numbers 1 to 8" << std::endl;;
        std::cin >> xcoord2 >> ycoord2; //TODO handle stupidity
        
        H.makemove(xcoord1-1,ycoord1-1,xcoord2-1,ycoord2-1);// Handle invalid move, return false
    }
    return 0;
}