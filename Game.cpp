//
//  Game.cpp
//  Chess Game
//
//  Created by Wade Sultan on 8/16/22.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include "Game.h"
#include "Piece.h"
#include "Player.h"
#include "Board.h"
using namespace std;

void Game::performEnPassant(Piece* bd[8][8], int iRank, int iFile, int fRank, int fFile){
    Piece* currPiece = bd[iRank][iFile];
    Piece* oppPiece = bd[iRank][fFile];
    
    // Moves pawn
    bd[fRank][fFile] = currPiece;
    bd[iRank][iFile] = 0;
    
    // Captures opposing pawn
    delete oppPiece;
    bd[iRank][fFile] = 0;
}
void Game::performCastle(Piece* bd[8][8], int iRank, int iFile, int fFile, bool kingside, char pTurn){
    if (kingside){
        // Move king
        bd[iRank][iFile + 2] = bd[iRank][iFile];
        bd[iRank][iFile] = 0;
        // Move rook
        bd[iRank][iFile + 1] = bd[iRank][7];
        bd[iRank][7] = 0;
    }
    else {
        // Move king
        bd[iRank][iFile - 2] = bd[iRank][iFile];
        bd[iRank][iFile] = 0;
        // Move rook
        bd[iRank][iFile - 1] = bd[iRank][0];
        bd[iRank][0] = 0;
    }
}

int Game::evalMoveType(Piece* cb[8][8], int iRank, int iFile, int fRank, int fFile){
    Piece* currPiece = cb[iRank][iFile];
    if (currPiece->getName() == "King" && abs(fFile - iFile) == 2){ // We already checked that castling is valid, and it is the only time when the king can move two files over.
        return CASTLE;
    }
    if (currPiece->getName() == "Pawn" && cb[fRank][fFile] == 0 && cb[iRank][fFile] != 0){
        Piece* oppPiece = cb[iRank][fFile];
        if (oppPiece->getName() == "Pawn" && oppPiece->getColor() != currPiece->getColor()){
            return EN_PASSANT;
        }
    }
    if (cb[fRank][fFile] != 0){
        return CAPTURE_MOVE;
    }
    return NORMAL_MOVE;
}

void Game::startGame(){
    gameBoard.display(false);
    while (!gameOver()){
        nextMove(gameBoard.c_board, gameBoard.l_board, gameBoard.p_board);
        turn();
    }
}
void Game::movePiece(Piece* cb[8][8], Player* pb[2], int iRank, int iFile, int fRank, int fFile, int pNum){
    bool toPromote = false;
    bool inPromotion = true;
    Piece* temp = cb[fRank][fFile];
    cb[fRank][fFile] = cb[iRank][iFile];
    cb[iRank][iFile] = temp;
    if (cb[iRank][iFile] != 0){
        delete temp;
        cb[iRank][iFile] = 0;
    }
    //Promote pawn if necessary.
    if (cb[fRank][fFile]->getName() == "Pawn"){
        Piece* pawn = cb[fRank][fFile];
        char pieceType;
        if (pawn->getColor() == 'W' && fRank == 7){
            toPromote = true;
        }
        if (pawn->getColor() == 'B' && fRank == 0){
            toPromote = true;
        }
        if (toPromote){
            while (inPromotion){
                cout<<"What type of piece are you promoting your pawn to? (r, n, b, q): ";
                cin>>pieceType;
                if (pieceType != 'Q' && pieceType != 'q' && pieceType != 'R' && pieceType != 'r' && pieceType != 'N' && pieceType != 'n' && pieceType != 'B' && pieceType != 'b'){
                    cout<<"That is not a valid promotion.";
                    cin>>pieceType;
                }
                pawn->promotePawn(cb, pieceType, fRank, fFile);
                inPromotion = false;
            }
        }
    }
    pb[pNum]->outOfCheck(); // If a move is successful, it got the player out of check.
}

void Game::nextMove(Piece* cb[8][8], Legal* lb[8][8], Player* pb[2]){
    
    bool checks[2] = {false,false};
    int kingRow = 0;
    int kingFile = 0;
    bool legalMoves = false;
    int playerNum;
    if (playerTurn == 'W')
        playerNum = 0;
    else
        playerNum = 1;
    
    // Finds king
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (cb[i][j] != 0 && cb[i][j]->getName() == "King" && cb[i][j]->getColor() == playerTurn){
                kingRow = i;
                kingFile = j;
            }
        }
    }
    Piece* king = cb[kingRow][kingFile];
    
    // Is it in check?
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (king->inCheck(cb, pb, playerTurn, -1, -1)){
                checks[playerNum] = true;
                pb[playerNum]->intoCheck();
                cout<<pb[playerNum]->getPlayerColor()<<" is in check!"<<endl;
                break;
            }
        }
        if (checks[playerNum]) break;
    }
    
    // Does the player have any legal moves?
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (cb[i][j] != 0 && cb[i][j]->getColor() == playerTurn){
                if (cb[i][j]->hasLegalMove(cb, pb, i, j, playerTurn)){
                    legalMoves = true;
                    break;
                }
            }
        }
        if (legalMoves)
            break;
    }
    
    // If there are no legal moves, end the game.
    if (!legalMoves){
        if (pb[playerNum]->checkStatus()){
            cout<<"Checkmate! "<<pb[1-playerNum]->getPlayerColor()<<" wins!"<<endl;
            // Opposite player of the player in check wins
        }
        else {
            cout<<"Stalemate!"<<endl;
        }
        endGame();
        return;
    }
    // Erasing legal moves from checking if any pieces had legal moves.
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (cb[i][j] != 0){
                cb[i][j]->eraseLegalMoves();
                }
            }
        }
    
    bool inTurn = true;
    int mType = 0;
    while (inTurn){
        string initial;
        cout<<pb[playerNum]->getPlayerColor()<<", choose a piece to move: ";
        getline(cin, initial);
        int initRank = convertChess(initial).first;
        int initFile = convertChess(initial).second;
        Piece* currPiece = cb[initRank][initFile];
        bool badMove = true;
        while (badMove){
            if (initRank == -1){
                cout<<"You did not enter a valid move (ex. e2). Select another square: ";
                getline(cin, initial);
                initRank = convertChess(initial).first;
                initFile = convertChess(initial).second;
                currPiece = cb[initRank][initFile];
            }
            else if (currPiece == nullptr){
                cout<<"There is no piece there! Select another square: ";
                getline(cin, initial);
                initRank = convertChess(initial).first;
                initFile = convertChess(initial).second;
                currPiece = cb[initRank][initFile];
            }
            else if (currPiece->getColor() != playerTurn){
                cout<<"That piece is not yours! Select another piece: ";
                getline(cin, initial);
                initRank = convertChess(initial).first;
                initFile = convertChess(initial).second;
                currPiece = cb[initRank][initFile];
            }
            else if (!currPiece->hasLegalMove(cb, pb, initRank, initFile, playerTurn)){
                cout<<"That piece has no legal moves! Select another piece: ";
                getline(cin, initial);
                initRank = convertChess(initial).first;
                initFile = convertChess(initial).second;
                currPiece = cb[initRank][initFile];
            }
            else {
                for (string i: currPiece->getLegalMoves()){
                    int r = convertChess(i).first;
                    int c = convertChess(i).second;
                    mType = evalMoveType(cb, initRank, initFile, r, c);
                    switch (mType){
                        case CAPTURE_MOVE:
                            gameBoard.addLegalMove(CAPTURE_MOVE, cb, r, c, reverseConvert(r, c));
                            break;
                        case EN_PASSANT:
                            gameBoard.addLegalMove(EN_PASSANT, cb, r, c, reverseConvert(r, c));
                            break;
                        case CASTLE:
                            gameBoard.addLegalMove(CASTLE, cb, r, c, reverseConvert(r, c));
                            break;
                        default:
                            gameBoard.addLegalMove(NORMAL_MOVE, cb, r, c, reverseConvert(r, c));
                            break;
                    }
                }
                badMove = false;
            }
        }
        gameBoard.display(true);
        gameBoard.displayCapMoves();
        cout<<"Now choose where you want to move the "<<currPiece->getName()<<" on "<<initial<<" to: ";
        cout<<endl<<"You may also enter 'x' to select another piece.";
        string dest;
        getline(cin, dest);
        if (dest == "x"){
            gameBoard.clearLegalMoves(lb);
            gameBoard.display(true);
            continue;
        }
        int finalRank = convertChess(dest).first;
        int finalFile = convertChess(dest).second;
        badMove = true;
        char pType = 'x';
        while (badMove){
            if (finalRank == -1){
                cout<<"Invalid input! Select another square to move to: ";
                getline(cin, dest);
                if (dest == "x"){
                    gameBoard.clearLegalMoves(lb);
                    gameBoard.display(true);
                    break;
                }
                finalRank = convertChess(dest).first;
                finalFile = convertChess(dest).second;
            }
            if (!cb[initRank][initFile]->isLegalMove(cb, initRank, initFile, finalRank, finalFile)){
                cout<<"That was not a legal move! Select another square to move to: ";
                getline(cin, dest);
                if (dest == "x"){
                    gameBoard.clearLegalMoves(lb);
                    gameBoard.display(true);
                    break;
                }
                finalRank = convertChess(dest).first;
                finalFile = convertChess(dest).second;
            }
            else {
                for (int i = 0; i<8; i++){
                    for (int j = 0; j<8; j++){
                        if (cb[i][j] != 0 && cb[i][j]->getColor() == playerTurn)
                            cb[i][j]->eraseLegalMoves();
                    }
                }
                mType = evalMoveType(cb, initRank, initFile, finalRank, finalFile);
                pType = currPiece->getName()[0];
                switch (mType){
                    case CASTLE:
                        performCastle(cb, initRank, initFile, finalFile, finalFile>initFile, playerTurn);
                        break;
                    case EN_PASSANT:
                        performEnPassant(cb, initRank, initFile, finalRank, finalFile);
                        break;
                    default:
                        movePiece(cb, pb, initRank, initFile, finalRank, finalFile, playerNum);
                        break;
                }
                badMove = false;
                
            }
            assert(pType != 'x');
            gameBoard.addMove(pType, dest);
            moveCount++; // Increases the global move count
            pb[playerNum]->incMoveCount(); // Increments the current player's move count.
            currPiece->addMoveToHist(initial,dest,moveCount);
            gameBoard.clearLegalMoves(lb);
            inTurn = false;
        }
    }
    gameBoard.display(false);
    
    // Old test data to verify that convertChess and reverseConvert work as intended.
    assert(convertChess("e4").first == 3 && convertChess("e4").second == 4);
    assert(convertChess("a1").first == 0 && convertChess("a1").second == 0);
    assert(convertChess("h8").first == 7 && convertChess("h8").second == 7);
    assert(convertChess("G2").first == 1 && convertChess("G2").second == 6); // Checks for capital letter input
    assert(convertChess("x9").first == -1 && convertChess("x9").second == -1); // Bad input

    assert(reverseConvert(3, 4) == "e4");
    assert(reverseConvert(7, 0) == "a8");
    assert(reverseConvert(0, 7) == "h1");
    assert(reverseConvert(9,10) == "bad");
}
void Game::turn(){
    playerTurn = (playerTurn == 'W') ? 'B' : 'W';
}


