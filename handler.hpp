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
    Color getcurrentcolor(){
        return currentcolor;
    }
    bool makemove(char xpos,char ypos,char xdes,char ydes){
        Piece* destOriginal = Board[xdes][ydes];
        bool undo = false;
        int offset = (currentcolor == WHITE) ? 32 : 0;
        int coloffset = (currentcolor == WHITE) ? 0 : 7;
        int specialmove;
        //Check you only move your pieces
        if (Board[xpos][ypos]->getcolor() != currentcolor)
            return false;
        //Check that the target positon is not occupied by one of your pieces
        if(currentcolor == destOriginal->getcolor())
            return false;
        //Then we will check if the move is valid, either by being a valid move or a valid special move.
        if (Board[xpos][ypos]->move(xdes,ydes) == true) {
            //Make the move
            Board[xdes][ydes]=Board[xpos][ypos];
            Board[xpos][ypos]=new Empty(xpos,ypos,NONE);
            if (incheck(currentcolor)){  //Are we in check after the move, if so restore position
                undo = true;
            } else{
                char origPiece = Board[xdes][ydes]->gettype();
                char offset = (currentcolor == BLACK) ? 0 : 32;
                //did we go through pieces during the move? this has to be checked for B,R,Q
                int nx = xpos - xdes;
                int ny = ypos - ydes;
                int dx,dy,x,y,i;
                //B or Q, if Q moved like a R then it goes to the next if
                if((origPiece == ('b' - offset) || origPiece == ('q' - offset)) && nx !=0 && ny !=0){
                    dx = (xdes > xpos) ? 1 : -1;
                    dy = (ydes > ypos) ? 1 : -1;
                    i = 1;
                    x = xpos + i*dx;
                    y = ypos + i*dy;
                    while (x!=xdes){
                        if(Board[x][y]->gettype() != '-'){
                            undo = true;
                            break;
                        }
                        i++;
                        x = xpos + i*dx;
                        y = ypos + i*dy;
                    }
                }
                //R or Q
                if(undo == false && (origPiece == ('r' - offset) || origPiece == ('q' - offset))){
                    dx = (nx == 0) ? 0 : (nx > 0 ? -1 : 1);
                    dy = (ny == 0) ? 0 : (ny > 0 ? -1 : 1);
                    x = xpos + dx;
                    y = ypos + dy;
                    while (x != xdes || y != ydes) {
                        if (Board[x][y]->gettype() != '-') {
                            undo = true;
                            break;
                        }
                        x += dx;
                        y += dy;
                    }                
                }
            }
            if (undo == true){
                Board[xpos][ypos]->setX(xpos);
                Board[xpos][ypos]->setY(ypos);
                Board[xpos][ypos]=Board[xdes][ydes];
                Board[xdes][ydes]= destOriginal;
                return false;
            }
            Board[xdes][ydes]->setX(xdes);
            Board[xdes][ydes]->setY(ydes);
            char mpiece = Board[xdes][ydes]->gettype();
            if (mpiece==('p'-offset)|| mpiece==('r'-offset) || mpiece==('k'-offset)){
                Board[xdes][ydes]->sethasmoved(true);
                if (mpiece == ('p'- offset) && xdes == (0 + coloffset))
                    promotion(xdes,ydes);//TODO: Not checked
            }
        }else if (Board[xpos][ypos]->specialmove(xdes,ydes) != 0) {
            specialmove = Board[xpos][ypos]->specialmove(xdes,ydes);
            if(Board[xpos][ypos]->gettype()==('p'- offset)){
                return (specialmove) ? specialmove : (Board[xdes][ydes]->getcolor()!=NONE);
            }
            if(Board[xpos][ypos]->gettype()==('k'- offset)){ //TODO: Exhaustive test, checks
                return castle(xpos,ypos,specialmove,offset);
            }
        }else{
            return false;
        }
        currentcolor = (currentcolor == WHITE) ? BLACK : WHITE; // Update current color playing after a valid move
        return true;
    }
    void promotion(int xdes,int ydes){
        char piece;
        bool again = false;
        do {
            again = false;
            std::cout << "What piece do you want to promote to? (r/R,b/B,n/N,q/Q)" << std::endl;
            std::cin >> piece;
            switch (piece) {
                case 'r':
                case 'R':
                    Board[xdes][ydes]=new Rook(xdes,ydes,currentcolor);
                break;
                case 'b':
                case 'B':
                    Board[xdes][ydes]=new Bishop(xdes,ydes,currentcolor);
                break;
                case 'n':
                case 'N':
                    Board[xdes][ydes]=new Knight(xdes,ydes,currentcolor);
                break;
                case 'q':
                case 'Q':
                    Board[xdes][ydes]=new Queen(xdes,ydes,currentcolor);
                break;
            default:
                std::cout << "Invalid option!!"<< std::endl;
                again = true;
                break;
            }
        } while (again);
    }
    bool castle(int xpos, int ypos, int castletype, int offset){
        int kingside = castletype - 1;
        // Define parameters depending on side
        int rookStartX   = kingside ? 7 : 0;
        int kingTargetX  = kingside ? 6 : 2;
        int rookTargetX  = kingside ? 5 : 3;
        int kingPath[2]  = { kingside ? 5 : 3, kingside ? 6 : 2 }; // king passes through these
        int coloffset = (currentcolor == WHITE) ? 0 : 6;

        // Empty squares between king and rook
        int emptySquaresKingside[2] = {5, 6};
        int emptySquaresQueenside[3] = {1, 2, 3};

        int* emptySquares = kingside ? emptySquaresKingside : emptySquaresQueenside;
        int emptyCount    = kingside ? 2 : 3;
        
        // 1. Check that the rook is in the correct place
        if (Board[rookStartX][coloffset]->gettype() != ('r' - offset))
            return false;

        // 2. Neither rook nor king has moved
        if (Board[rookStartX][coloffset]->gethasmoved())
            return false;

        // 3. No pieces between king and rook
        for (int i = 0; i < emptyCount; i++) {
            if (Board[emptySquares[i]][coloffset]->gettype() != '-')
                return false;
        }

        // 4. King must not be in check at any stage
        if (incheck(currentcolor)) return false;
        for (int i = 0; i < 2; i++) {
            Board[kingPath[i]][coloffset] = Board[xpos][ypos];
            if (incheck(currentcolor)) {
                // cleanup
                Board[kingPath[0]][coloffset] = new Empty(kingPath[0], coloffset, NONE);
                Board[kingPath[1]][coloffset] = new Empty(kingPath[1], coloffset, NONE);
                return false;
            }
        }

        // Complete the move
        Board[kingTargetX][coloffset] = Board[xpos][ypos];
        Board[kingTargetX][coloffset]->setX(kingTargetX);

        Board[rookTargetX][coloffset] = Board[rookStartX][coloffset];
        Board[rookTargetX][coloffset]->setX(rookTargetX);
        // Clear old positions
        Board[4][coloffset] = new Empty(4, coloffset, NONE);
        Board[rookStartX][coloffset] = new Empty(rookStartX, coloffset, NONE);

        Board[rookTargetX][coloffset]->sethasmoved(true);
        Board[kingTargetX][coloffset]->sethasmoved(true);
        return true;
    }
    int findPiece(char t){
        char piece;
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                piece = Board[x][y]->gettype();
                if (piece == t) {
                    return x|(y<<8);
                }
            }
        }
        return 0;
    }
    bool inBounds(int x){
        return (x>=0 && x<8);
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