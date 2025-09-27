#include "piece.hpp"
#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/empty.hpp"
#include "pieces/pawn.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"
class Handler
{
private:
    Piece* Board[8][8];
    char movecounter;
    Color currentcolor; // 0-white 1-black
    //TODO add we are in check variables
public:
    Handler(){
        //Create Pawns and Nullpieces
        for (int i = 0; i < 8; i++) {
            Board[i][6]=new Pawn(i,6,BLACK);
            Board[i][1]=new Pawn(i,1,WHITE);
            Board[i][2]=new Empty(i,2,NONE);
            Board[i][3]=new Empty(i,3,NONE);
            Board[i][4]=new Empty(i,4,NONE);
            Board[i][5]=new Empty(i,5,NONE);
        }
        //Create Knights
        Board[6][0]=new Knight(6,0,WHITE);
        Board[1][0]=new Knight(1,0,WHITE);
        Board[6][7]=new Knight(6,7,BLACK);
        Board[1][7]=new Knight(1,7,BLACK);
        //Create Bishops
        Board[5][0]=new Bishop(5,0,WHITE);
        Board[2][0]=new Bishop(2,0,WHITE);
        Board[5][7]=new Bishop(5,7,BLACK);
        Board[2][7]=new Bishop(2,7,BLACK);
        //Create Rooks
        Board[0][0]=new Rook(0,0,WHITE);
        Board[7][0]=new Rook(7,0,WHITE);
        Board[0][7]=new Rook(0,7,BLACK);
        Board[7][7]=new Rook(7,7,BLACK);
        //Create Queens
        Board[3][0]=new Queen(3,0,WHITE);
        Board[3][7]=new Queen(3,7,BLACK);
        //Create Kings
        Board[4][0]=new King(4,0,WHITE);
        Board[4][7]=new King(4,7,BLACK);

        currentcolor = WHITE;
        movecounter = 0;
    }
    void printboard(){
        std::cout << "  1 2 3 4 5 6 7 8";
        for (int i = 7; i > -1; i--) {
                std::cout << std::endl;
                std::cout <<i + 1<<" ";
                for (int j = 0; j < 8; j++) {
                    std::cout << Board[j][i]->gettype()<<" ";
                }
            }
            std::cout << std::endl;
    }
    Piece* (&getBoard())[8][8] {
        return Board;
    }
    bool makemove(char xpos,char ypos,char xdes,char ydes){
        if (Board[xpos][ypos]->move(xdes,ydes) == true) {
            if (!incheck(currentcolor)){  //Are we in check after move /TODO
                //did we go through pieces, are there pieces at target? /TODO
                Board[xpos][ypos]->setX(xdes);
                Board[xpos][ypos]->setY(ydes);
                Board[xdes][ydes]=Board[xpos][ypos];
                Board[xpos][ypos]=new Empty(xpos,ypos,NONE);
                return true;
            }
        }else if (Board[xpos][ypos]->specialmove(xdes,ydes) == true){
            //Are we in check after move /TODO
            //did we go through pieces, are there pieces at target? /TODO
            //Special conditions depending on the piece /TODO
            Board[xdes][ydes]=Board[xpos][ypos];
        }
        return false;
    }
    int findPiece(char t){
        char piece;
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                piece = Board[x][y]->gettype();
                if (piece == t) {
                    return x|(y<<8); //TODO igual falla
                }
            }
        }
        return 0;
    }
    bool inBounds(int x){
        return (x>=0 && x<9);
    }
    bool incheck(Color col){
        char offset = (col == WHITE) ? 0 : 32;
        int xy,x,y,nx,ny;
        int a,b,c,d;
        char piece;
        //Get King position
        xy=findPiece('K' + offset);
        x=xy&0xff;
        y=(xy >> 8) & 0xFF;
        //Conditions to be in check(idea, check from kings perspective)
        //N 8 Moves -1-2 -1+2 +1-2 +1+2 -2+1 -2-1 +2+1 +2+1
        int knightMoves[8][2] = {
            {-2, -1}, {-2, +1},{-1, -2}, {-1, +2},
            {+1, -2}, {+1, +2},{+2, -1}, {+2, +1}
        };
        for (auto &move : knightMoves) {
            nx = x + move[0];
            ny = y + move[1];
            if (inBounds(nx) && inBounds(ny) && Board[nx][ny]->gettype() == ('n' - offset)) {
                return true;
            }
        }
        //B or Q - All moves until collision with another piece
        int bishopMoves[4][2] = {
            {1,1},{1,-1},{-1,1},{-1,-1}
        };
        for (auto &move : bishopMoves) {
            a=1;
            nx = x + a*move[0];
            ny = y + a*move[1];
            while (inBounds(nx) && inBounds(ny)){
                piece = Board[nx][ny]->gettype();
                if (piece == ('b' - offset) || piece == ('q' - offset)) {
                    return true;
                }
                if(piece != '-')
                    break;
                a++;
                nx = x + a*move[0];
                ny = y + a*move[1];
            }
        }
        //R or Q - All moves until collision with another piece
        int rookMoves[4][2] = {
            {0,1},{0,-1},{-1,0},{1,0}
        };
        for (auto &move : rookMoves) {
            a=1;
            nx = x + a*move[0];
            ny = y + a*move[1];
            while (inBounds(nx) && inBounds(ny)){
                piece = Board[nx][ny]->gettype();
                if (piece == ('r' - offset) || piece == ('q' - offset)) {
                    return true;
                }
                if(piece != '-')
                    break;
                a++;
                nx = x + a*move[0];
                ny = y + a*move[1];
            }
        }
        //P      - Diagonal, just two moves, depends on the color
        int pawnMove = (col==WHITE)? 1:-1;
        if(Board[x+1][y+pawnMove]->gettype()==('p' - offset)){
            return true;
        }
        if(Board[x-1][y+pawnMove]->gettype()==('p' - offset)){
            return true;
        }
        return false;
    }
    bool checkMate(){
        return false;
    }
    bool tie(){
        return false;
    }
};