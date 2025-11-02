#include "handler.hpp"
#include <string.h>
// #include "raylib.h"

int main(){
    bool win = false;
    bool tie = false;
    bool check = false;
    //Board and Pieces initialization
    Handler H;
    //Game loop
    int xcoord1,ycoord1,xcoord2,ycoord2;

    char color[6]; 
    //7 2 7 3 2 7 2 6 7 1 6 3 2 8 3 6 6 1 7 2 3 8 2 7
    //2 2 2 3 2 7 2 6 2 1 3 3 2 6 2 5 3 1 2 2 3 8 1 6 4 2 4 3 2 5 2 4 4 1 4 2
    // 6 2 6 3 5 7 5 6 7 2 7 4 4 8 8 4
    // 6 2 6 3 5 7 5 6 2 1 3 3
    while (!win && !tie) {
        H.printboard();
        
        //TODO: Hide menu
        strcpy(color, (H.getcurrentcolor() == W) ? "WHITE" : "BLACK");
        std::cout << color <<" to play";
        if(check) // Save the positon of piece the giving check
            std::cout << ": You are in Check!!";
        std::cout << std::endl << "What piece do you want to move? xy, numbers 1 to 8" << std::endl;
        std::cin >> xcoord1 >> ycoord1; //TODO handle stupidity
        std::cout << "Where do you want to move your " << H.getBoard()[xcoord1-1][ycoord1-1]->gettype() << "? xy, numbers 1 to 8" << std::endl;;
        std::cin >> xcoord2 >> ycoord2; //TODO handle stupidity
        H.makemove(xcoord1-1,ycoord1-1,xcoord2-1,ycoord2-1,false);// Handle invalid move, return false
        check = H.incheck(H.getcurrentcolor(),true);
        if (check)
            win = H.checkMate(); 
        tie = H.tie();
    }
    H.printboard();
    if (win) {
        strcpy(color, (H.getcurrentcolor() == W) ? "BLACK" : "WHITE");
        std::cout << "Checkmate, " << color << " WON!!!!";
    }
    return 0;
}