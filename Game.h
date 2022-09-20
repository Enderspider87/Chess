//
//  Game.h
//  Chess Game
//
//  Created by Wade Sultan on 8/12/22.
//

#ifndef Game_h
#define Game_h
#include "Piece.h"
#include "Player.h"
#include "Board.h"

class Game {
public:
    Game(): playerTurn('W'), isOver(false){}
    // Game constructor. Initialzies playerTurn to W because white always plays first, and isOver is initialized to false.
    
    ~Game(){} // Game destructor
    
    void startGame(); // Starts the game
    
    void endGame(){ // Sets isOver to true, which will end the game
        isOver = true;
    }
    
    void nextMove(Piece* cb[8][8], Legal* lb[8][8], Player* pb[2]);
    void selectPiece();
    void selectMove();
    void movePiece(Piece* cb[8][8], Player* pb[2], int iRank, int iFile, int fRank, int fFile, int pNum);
    void displayCaptureMoves(Piece* cb[8][8]);
    int evalMoveType(Piece* cb[8][8], int iRank, int iFile, int fRank, int fFile);
    void performEnPassant(Piece* bd[8][8], int iRank, int iFile, int fRank, int fFile);
    // Specialized move function for en passant.
    void performCastle(Piece* bd[8][8], int iRank, int iFile, int fFile, bool kingside, char pTurn);
    // Castles. Uses pTurn to determine which color, and bool kingside to determine which side.
    bool gameOver(){
        return isOver;
    }
    void turn();
    int getMoveCount(){
        return moveCount;
    }
    char whoseTurn(){
        return playerTurn;
    }
    
private:
    char playerTurn;
    int moveCount;
    Board gameBoard;
    bool isOver;
    

};

// These functions convert to and from chess notation to make it easier for humans to play.
inline pair<int,int> convertChess(string square){
    int x = 0;
    int y = 0;
    
    switch(square[0]){ // Determine file
        case 'a':
        case 'A': x = 0; break;
        case 'b':
        case 'B': x = 1; break;
        case 'c':
        case 'C': x = 2; break;
        case 'd':
        case 'D': x = 3; break;
        case 'e':
        case 'E': x = 4; break;
        case 'f':
        case 'F': x = 5; break;
        case 'g':
        case 'G': x = 6; break;
        case 'h':
        case 'H': x = 7; break;
        default: return make_pair(-1,-1); // Bad letter passed in
    }
    if (square[1]-'0' > 0 && square[1]-'0' < 9) // Determines rank
        y = square[1]-'1';
    else
        return make_pair(-1,-1); // bad number
    
    return make_pair(y,x);
}
inline string reverseConvert(int r, int c){
    string chesspos = "x0"; // Bogus initial position
    switch(r){
        case 0: chesspos[1] = '1'; break;
        case 1: chesspos[1] = '2'; break;
        case 2: chesspos[1] = '3'; break;
        case 3: chesspos[1] = '4'; break;
        case 4: chesspos[1] = '5'; break;
        case 5: chesspos[1] = '6'; break;
        case 6: chesspos[1] = '7'; break;
        case 7: chesspos[1] = '8'; break;
        default: chesspos = "bad"; break;
    }
    switch(c){
        case 0: chesspos[0] = 'a'; break;
        case 1: chesspos[0] = 'b'; break;
        case 2: chesspos[0] = 'c'; break;
        case 3: chesspos[0] = 'd'; break;
        case 4: chesspos[0] = 'e'; break;
        case 5: chesspos[0] = 'f'; break;
        case 6: chesspos[0] = 'g'; break;
        case 7: chesspos[0] = 'h'; break;
        default: chesspos = "bad"; break;
    }
    return chesspos;
}

#endif /* Game_h */
