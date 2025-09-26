#include "handler.hpp"
#include "pieces/bishop.hpp"

int main(){
    //Board and Pieces initialization
    Handler H;
    //Game loop
    bool win = false;
    bool tie = false;
    int xcoord1,ycoord1,xcoord2,ycoord2;
    while (win == false && tie==false) {
        H.printboard();
        //TODO: Hide menu
        std::cout << "What piece do you want to move? xy, numbers 1 to 8";
        std::cin >>xcoord1>>ycoord1; //TODO handle stupidity
        std::cout << "Where do you want to move your "<<H.getBoard()[xcoord1-1][ycoord1-1]->gettype()<< "? xy, numbers 1 to 8";
        std::cin >>xcoord2>>ycoord2; //TODO handle stupidity
        H.makemove(xcoord1-1,ycoord1-1,xcoord2-1,ycoord2-1);// Handle invalid move, return false
    }
    return 0;
}