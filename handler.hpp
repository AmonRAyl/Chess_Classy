#include "piece.hpp"
#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/empty.hpp"
#include "pieces/pawn.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"
#include <string>
#include <functional>
class Handler
{
private:
    Piece* Board[8][8];
    char movecounter;
    char tiemoveCounter;
    Colors currentcolor; // 0-white 1-black
    int enpassant[2];
    int xcheck,ycheck;
    size_t boardlist[50];
    char i_board;
public:
    Handler(){
        //Create Pawns and Nullpieces
        for (int i = 0; i < 8; i++) {
            Board[i][6]=new Pawn(i,6,B);
            Board[i][1]=new Pawn(i,1,W);
            Board[i][2]=new Empty(i,2,N);
            Board[i][3]=new Empty(i,3,N);
            Board[i][4]=new Empty(i,4,N);
            Board[i][5]=new Empty(i,5,N);
        }
        //Create Knights
        Board[6][0]=new Knight(6,0,W);
        Board[1][0]=new Knight(1,0,W);
        Board[6][7]=new Knight(6,7,B);
        Board[1][7]=new Knight(1,7,B);
        //Create Bishops
        Board[5][0]=new Bishop(5,0,W);
        Board[2][0]=new Bishop(2,0,W);
        Board[5][7]=new Bishop(5,7,B);
        Board[2][7]=new Bishop(2,7,B);
        //Create Rooks
        Board[0][0]=new Rook(0,0,W);
        Board[7][0]=new Rook(7,0,W);
        Board[0][7]=new Rook(0,7,B);
        Board[7][7]=new Rook(7,7,B);
        //Create Queens
        Board[3][0]=new Queen(3,0,W);
        Board[3][7]=new Queen(3,7,B);
        //Create Kings
        Board[4][0]=new King(4,0,W);
        Board[4][7]=new King(4,7,B);

        currentcolor = W;
        movecounter = 0;
        tiemoveCounter = 0;
        
        enpassant[0]=-1;
        enpassant[1]=-1;
        
        xcheck=-1;
        ycheck=-1;

        std::string stBoard = stringBoard();
        std::hash<std::string> hasher;
        boardlist[0] = hasher(stBoard);
        i_board = 1;
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
    Colors getcurrentcolor(){
        return currentcolor;
    }
    bool makemove(char xpos,char ypos,char xdes,char ydes, bool testmove){
        Piece* destOriginal = Board[xdes][ydes];
        bool undo = false;
        int offset = (currentcolor == W) ? 32 : 0;
        int coloffset = (currentcolor == W) ? 0 : 7;
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
            Board[xpos][ypos]=new Empty(xpos,ypos,N);
            if (incheck(currentcolor,false)){  //Are we in check after the move, if so restore position
                undo = true;
            } else{
                char origPiece = Board[xdes][ydes]->gettype();
                char offset = (currentcolor == B) ? 0 : 32;
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
            }else{
                if (testmove){
                    Board[xpos][ypos]->setX(xpos);
                    Board[xpos][ypos]->setY(ypos);
                    Board[xpos][ypos]=Board[xdes][ydes];
                    Board[xdes][ydes]= destOriginal;
                    return true;
                }
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
                if (specialmove == 1){
                    Board[xdes][ydes] = Board[xpos][ypos];
                    Board[xpos][ypos] = new Empty(xpos,ypos,N);
                    //Check that the square in between the double move is empty, and that we are not in check after the double move
                    if (Board[xpos][(ypos+ydes)/2]->getcolor()!=N || incheck(currentcolor,false)) {
                        Board[xpos][ypos] = Board[xdes][ydes];
                        Board[xdes][ydes] = new Empty(xdes,ydes,N);
                        return false;
                    }
                    if(!testmove){
                        Board[xdes][ydes]->setX(xdes);
                        Board[xdes][ydes]->setY(ydes);
                        //Save what pawn has moved double in case of enpassant
                        enpassant[0]=xdes;
                        enpassant[1]=movecounter;
                    }else{
                        Board[xpos][ypos] = Board[xdes][ydes];
                        Board[xdes][ydes] = new Empty(xdes,ydes,N);
                        return false;
                    }
                }else{
                    if (!pawncaputre(xpos,ypos,xdes,ydes,testmove))
                        return false;
                    if(!testmove){
                        Board[xdes][ydes]->setX(xdes);
                        Board[xdes][ydes]->setY(ydes);
                    }
                }
            }else{
                if(Board[xpos][ypos]->gettype()==('k'- offset)){ //TODO: Exhaustive test, checks
                    if (!castle(xpos,ypos,specialmove,offset)){ //This function will not need to take into account if it is a testmove as this is only used when incheck
                        return false;
                    }
                }
            }
        }else{
            return false;
        }
        movecounter++;
        currentcolor = (currentcolor == W) ? B : W; // Update current color playing after a valid move
        if (destOriginal->getcolor() == currentcolor || Board[xdes][ydes]->gettype()==('p' - offset)) {
            tiemoveCounter = movecounter;
        }
        return true;
    }
    bool pawncaputre(int xpos,int ypos,int xdes,int ydes, bool testmove){
        Piece* capPiece = Board[xdes][ydes];
        if (Board[xdes][ydes]->getcolor()!=N){
            Board[xdes][ydes]=Board[xpos][ypos];
            Board[xpos][ypos] = new Empty(xpos,ypos,N);
            if(incheck(currentcolor,false)){
                Board[xpos][ypos]=Board[xdes][ydes];
                Board[xdes][ydes]=capPiece;
                return false;
            }
            if(testmove){
                Board[xpos][ypos]=Board[xdes][ydes];
                Board[xdes][ydes]=capPiece;
            }
            return true;
        }else{
            int coloffset = (currentcolor==W) ? 0 : 1 ;
            int coloffset2 = (currentcolor==W) ? -1 : 1 ;
            if (ypos == (4-coloffset) && enpassant[0]==xdes && enpassant[1]==(movecounter-1)){
                Board[xdes][ydes]=Board[xpos][ypos];
                Board[xpos][ypos] = new Empty(xpos,ypos,N);
                if(incheck(currentcolor,false)){
                    Board[xpos][ypos]=Board[xdes][ydes];
                    Board[xdes][ydes] = new Empty(xdes,ydes,N);
                    return false;
                }
                if(testmove){
                    Board[xpos][ypos]=Board[xdes][ydes];
                    Board[xdes][ydes] = new Empty(xdes,ydes,N);
                }else{
                    Board[xdes][ydes+coloffset2] = new Empty(xdes,ydes+coloffset2,N);
                }
                return true;
            }
        }
        return false;
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
        int coloffset = (currentcolor == W) ? 0 : 6;

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
        if (incheck(currentcolor,false)) return false;
        for (int i = 0; i < 2; i++) {
            Board[kingPath[i]][coloffset] = Board[xpos][ypos];
            if (incheck(currentcolor,false)) {
                // cleanup
                Board[kingPath[0]][coloffset] = new Empty(kingPath[0], coloffset, N);
                Board[kingPath[1]][coloffset] = new Empty(kingPath[1], coloffset, N);
                return false;
            }
        }

        // Complete the move
        Board[kingTargetX][coloffset] = Board[xpos][ypos];
        Board[kingTargetX][coloffset]->setX(kingTargetX);

        Board[rookTargetX][coloffset] = Board[rookStartX][coloffset];
        Board[rookTargetX][coloffset]->setX(rookTargetX);
        // Clear old positions
        Board[4][coloffset] = new Empty(4, coloffset, N);
        Board[rookStartX][coloffset] = new Empty(rookStartX, coloffset, N);

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
    bool incheck(Colors col, bool saveCheck){
        char offset = (col == W) ? 0 : 32;
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
                if (saveCheck){
                    xcheck=nx;
                    ycheck=ny;
                }
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
                    if (saveCheck){
                        xcheck=nx;
                        ycheck=ny;
                    }
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
                    if (saveCheck){
                        xcheck=nx;
                        ycheck=ny;
                    }
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
        int pawnMove = (col==W)? 1:-1;
        if(Board[x+1][y+pawnMove]->gettype()==('p' - offset)){
            if (saveCheck){
                xcheck=nx;
                ycheck=ny;
            }
            return true;
        }
        if(Board[x-1][y+pawnMove]->gettype()==('p' - offset)){
            if (saveCheck){
                xcheck=nx;
                ycheck=ny;
            }
            return true;
        }
        return false;
    }
    bool checkMate(){
        char offset = (currentcolor == W) ? 0 : 32;//Maybe it should be the other way around
        int xy,x,y,nx,ny;
        int xdir,ydir;
        Piece* checkpiece = Board[xcheck][ycheck];
        //Get King position
        xy=findPiece('K' + offset);
        x=xy&0xff;
        y=(xy >> 8) & 0xFF;
        // Conditions:
            // 1- King cannot move, if king can move away from check, the it is not mate
        int kingMoves[8][2] = {
            {0,1},{0,-1},{1,0},{1,1},{1,-1},{-1,0},{-1,1},{-1,-1}
        };
        for (auto &move : kingMoves) {
            nx = x + move[0];
            ny = y + move[1];
            if (inBounds(nx) && inBounds(ny)){
                if(makemove(x,y,nx,ny,true))
                    return false;
            }
        }        
            // 2- No piece can block the check (!!)
                //Go through the whole board searching for pieces that could block
        xdir = (x - xcheck < 0) ? 1: (x - xcheck > 0) ? 1 : 0;
        ydir = (y - ycheck < 0) ? 1: (y - ycheck > 0) ? 1 : 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if(Board[i][j]->getcolor()==currentcolor && Board[i][j]->gettype()!=('K' + offset)){
                    //Can we capture the piece, and not be in check after that?
                    if(makemove(i,j,xcheck,ycheck,true))
                        return false;
                    //Can we block the check with this piece, this will only work if the check is given by B Q R
                    if (checkpiece->gettype()==('b' - offset) || checkpiece->gettype()==('r' - offset) || checkpiece->gettype()==('q' - offset)){
                        nx = x + xdir;
                        ny = y + ydir;
                        while (nx != xcheck&& ny != ycheck) {
                            if(makemove(i,j,nx,ny,true))
                                return false;
                            nx = nx + xdir;
                            ny = ny + ydir;
                        }
                    }
                }
            }            
        }
        return true;
    }
    std::string stringBoard(){
        std::string s;
        s.reserve(64);
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j) {
                s += Board[i][j]->gettype();
            }
        }
        return s;
    }
    bool tie(){
        //Three possible ties
            // 1- Tie by repetition, idea use hash, remove list of hashes when a move makes it impossible to be repeat previous positions
            std::string stBoard = stringBoard();
            std::hash<std::string> hasher;
            boardlist[i_board] = hasher(stBoard);
            i_board++;

            // 2- Tie by 50 move rule, no pawn advancements and no captures for 50 moves
            if(movecounter - tiemoveCounter == 100) // 100, 50 each player
                return true;
            // 3- Stalemate
            // Idea, for pieces that can move ++squares, like b r q, its ok to only look one step in each direction
        return false;
    }
};