//
//  Piece.cpp
//  Chess Game
//
//  Created by Wade Sultan on 8/16/22.
//
#include <stdio.h>
#include <iostream>
#include <cassert>
#include "Piece.h"
#include "Game.h"
using namespace std;

bool Piece::isValidMove(Piece* bd[8][8], Player* pb[2], int iRank, int iFile, int fRank, int fFile, char pTurn){
    int pNum = 0;
    bool playersTurn[2] = {false,false};
    switch (pTurn){
        case 'B':
            pNum = 1;
            playersTurn[pNum] = true;
            break;
        default:
            playersTurn[pNum] = true;
            break;
    }
    Piece* temp = bd[fRank][fFile];
    bd[fRank][fFile] = bd[iRank][iFile];
    bd[iRank][iFile] = temp;
    
    int kingRow = 0;
    int kingFile = 0;
    
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (bd[i][j] != 0 && bd[i][j]->getName() == "King" && bd[i][j]->getColor() == pTurn){
                kingRow = i;
                kingFile = j;
            }
        }
    }
    Piece* king = bd[kingRow][kingFile];
    if (king->inCheck(bd, pb, pTurn, iRank, iFile)){
        Piece* temp2 = bd[iRank][iFile]; // Undo the move
        bd[iRank][iFile] = bd[fRank][fFile];
        bd[fRank][fFile] = temp2;
        return false;
    }
    Piece* temp2 = bd[iRank][iFile]; // Undo the move
    bd[iRank][iFile] = bd[fRank][fFile];
    bd[fRank][fFile] = temp2;
    return true;
}

bool Piece::hasLegalMove(Piece* bd[8][8], Player* pb[2], int iRank, int iFile, char pTurn){
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (i == iRank && j == iFile)
                continue;
            if (bd[iRank][iFile]->isSquareValid(bd, pb, pTurn, iRank, iFile, i, j) && bd[iRank][iFile]->isValidMove(bd, pb, iRank, iFile, i, j, pTurn))
                addLegalMove(reverseConvert(i,j));
        }
    }
    if (legalMove.empty())
        return false;
    return true;
}

bool Piece::isLegalMove(Piece* bd[8][8], int iRank, int iFile, int fRank, int fFile){
    bool lMove[8][8];
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            lMove[i][j] = false; // False by default
        }
    }
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            for (vector<string>::iterator it = legalMove.begin(); it != legalMove.end(); it++){
                if (i == convertChess(*it).first && j == convertChess(*it).second)
                    lMove[i][j] = true;
            }
        }
    }
    return lMove[fRank][fFile];
}

bool lineNoObstructions(Piece* bd[8][8], int iRank, int iFile, int fRank, int fFile){
    if (fRank == iRank){
        if (fFile > iFile){
            for (int i = iFile+1; i<fFile; i++){
                if (bd[fRank][i] != 0){
                    return false;
                }
            }
        }
        else {
            for (int i = fFile+1; i<iFile; i++){
                if (bd[fRank][i] != 0){
                    return false;
                }
            }
        }
        return true;
    }
    if (fFile == iFile){
        if (fRank > iRank){
            for (int i = iRank+1; i<fRank; i++){
                if (bd[i][fFile] != 0){
                    return false;
                }
            }
        }
        else {
            for (int i = fRank+1; i<iRank; i++){
                if (bd[i][fFile] != 0){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}
bool diagNoObstructions(Piece* bd[8][8], int iRank, int iFile, int fRank, int fFile){
    int rankDiff = fRank-iRank;
    int fileDiff = fFile-iFile;
    int i = 0;
    if (abs(rankDiff) != abs(fileDiff))
        return false;
    else { // This checks diagonals
        if (fRank > iRank){
            if (fFile > iFile){
                for (i = 1; i<rankDiff; i++){ // Top right
                    if (bd[iRank+i][iFile+i] != 0)
                        return false;
                }
            }
            else {
                for (i = 1; i<rankDiff; i++){ // Top left
                    if (bd[iRank+i][iFile-i] != 0)
                        return false;
                }
            }
        }
        else {
            if (fFile > iFile){ // Bottom right
                for (i = 1; i<abs(rankDiff); i++){
                    if (bd[iRank-i][iFile+i] != 0)
                        return false;
                }
            }
            else { // Bottom left
                for (i = 1; i<abs(rankDiff); i++){
                    if (bd[iRank-i][iFile-i] != 0)
                        return false;
                }
            }
        }
    }
    return true;
}

bool Pawn::isSquareValid(Piece* bd[8][8], Player* pb[2], char pTurn, int iRank, int iFile, int fRank, int fFile){
    if (fRank == -1){ // Checks for bad input
        return false;
    }
    if (iRank == fRank && iFile == fFile) // Piece can't remain on its current square
        return false;
    Piece* pawn = bd[iRank][iFile];
    if (bd[fRank][fFile] != 0 && bd[fRank][fFile]->getColor() == pawn->getColor()){
        return false;
    }
    if(pawn->getColor() == 'W'){
        if (fRank == iRank + 1) {
            if (fFile == iFile){
                if (bd[fRank][fFile] == 0) // Cannot capture straight
                    return true;
            }
            else if (fFile == iFile + 1 || fFile == iFile - 1){
                if (bd[fRank][fFile] != 0) // Can only capture if it is diagonal
                    return true;
                else if (iRank == 4 && bd[fRank][fFile] == 0 && bd[iRank][fFile] != 0 && bd[iRank][fFile]->getName() == "Pawn" && bd[iRank][fFile]->getColor() != bd[iRank][iFile]->getColor()){
                    return isEnPassantLegal(bd, pb, iRank, iFile, fRank, fFile);
                }
            }
        }
        if (iRank == 1 && fRank == 3 && fFile == iFile){
            if (bd[2][iFile] == 0 && bd[3][iFile] == 0)
                return true;
        }
        else
            return false;
    }
    else if (pawn->getColor() == 'B'){
        if (fRank == iRank - 1) {
            if (fFile == iFile && bd[fRank][fFile] == 0){
                return true;
            }
            else if (fFile == iFile + 1 || fFile == iFile - 1){
                if (bd[fRank][fFile] != 0) // Can only capture if it is diagonal
                    return true;
                else if (iRank == 3 && bd[fRank][fFile] == 0 && bd[iRank][fFile] != 0 && bd[iRank][fFile]->getName() == "Pawn" && bd[iRank][fFile]->getColor() != bd[iRank][iFile]->getColor()){
                    return isEnPassantLegal(bd, pb, iRank, iFile, fRank, fFile);
                }
            }
        }
        if (iRank == 6 && fRank == 4 && fFile == iFile){
            if (bd[5][iFile] == 0 && bd[4][iFile] == 0)
                return true;
        }
        else
            return false;
    }
    return false;
}
bool Pawn::isEnPassantLegal(Piece* bd[8][8], Player* pb[2], int iRank, int iFile, int fRank, int fFile){
    // Already checked: Pawn is on appropriate rank, and there is a pawn to the left or right, on the pawn's final file if the capture were to happen.
    Piece* oppPawn = bd[iRank][fFile];
    if (oppPawn->getMoveHist().size() != 1){
        return false; // Piece has moved more than once, which is not allowed in en passant
    }
    
    // Three variables to fetch from
    string initSq;
    string finalSq;
    int moveNum = 0;
    for (auto [a,b,c] : oppPawn->getMoveHist()){
        cout<<a<<", "<<b<<", "<<c<<endl;
        initSq = a;
        finalSq = b;
        moveNum = c;
    }
    int totalMoveCount = pb[0]->getMoveCount() + pb[1]->getMoveCount();
    
    if (initSq[0] == finalSq[0] && abs(initSq[1] - finalSq[1]) == 2){
        if (moveNum == totalMoveCount) // If the last move was the pawn move, en passant is legal
            return true;
    }
    return false;
}

bool Rook::isSquareValid(Piece* bd[8][8], Player* pb[2], char pTurn, int iRank, int iFile, int fRank, int fFile){
    if (fRank == -1)
        return false;
    if (iRank == fRank && iFile == fFile)
        return false;
    Piece* rook = bd[iRank][iFile];
    if (bd[fRank][fFile] != 0 && bd[fRank][fFile]->getColor() == rook->getColor()){
        return false;
    }
    if (lineNoObstructions(bd, iRank, iFile, fRank, fFile))
        return true;
    return false;
}

bool Knight::isSquareValid(Piece* bd[8][8], Player* pb[2], char pTurn, int iRank, int iFile, int fRank, int fFile){
    if (fRank == -1)
        return false;
    if (iRank == fRank && iFile == fFile)
        return false;
    Piece* knight = bd[iRank][iFile];
    if (bd[fRank][fFile] != 0 && bd[fRank][fFile]->getColor() == knight->getColor()){
        return false;
    }
    if (fRank == iRank + 2 || fRank == iRank - 2){
        if (fFile == iFile + 1 || fFile == iFile - 1){
            return true;
        }
    }
    else if (fRank == iRank + 1 || fRank == iRank - 1){
        if (fFile == iFile + 2 || fFile == iFile - 2){
            return true;
        }
    }
    return false;
}

bool Bishop::isSquareValid(Piece* bd[8][8], Player* pb[2], char pTurn, int iRank, int iFile, int fRank, int fFile){
    if (fRank == -1)
        return false;
    if (iRank == fRank || iFile == fFile)
        return false;
    Piece* bishop = bd[iRank][iFile];
    if (bd[fRank][fFile] != 0 && bd[fRank][fFile]->getColor() == bishop->getColor()){
        return false;
    }
    if (diagNoObstructions(bd, iRank, iFile, fRank, fFile))
        return true;
    return false;
}

bool Queen::isSquareValid(Piece* bd[8][8], Player* pb[2], char pTurn, int iRank, int iFile, int fRank, int fFile){
    if (fRank == -1)
        return false;
    if (iRank == fRank && iFile == fFile)
        return false;
    Piece* queen = bd[iRank][iFile];
    if (bd[fRank][fFile] != 0 && bd[fRank][fFile]->getColor() == queen->getColor()){
        return false;
    }
    if (fRank == iRank || fFile == iFile){
        if (lineNoObstructions(bd, iRank, iFile, fRank, fFile))
            return true;
    }
    else {
        if (diagNoObstructions(bd, iRank, iFile, fRank, fFile))
            return true;
    }
    return false;
}

bool King::isSquareValid(Piece* bd[8][8], Player* pb[2], char pTurn, int iRank, int iFile, int fRank, int fFile){
    if (fRank == -1)
        return false;
    Piece* king = bd[iRank][iFile];
    if (bd[fRank][fFile] != 0 && bd[fRank][fFile]->getColor() == king->getColor()){
        return false;
    }
    if (iRank == fRank && iFile == fFile)
        return false;
    int y = abs(iRank - fRank);
    int x = abs(iFile - fFile);
    if (x == 1 || x == 0){
        if (y == 1 || y == 0){
            return true;
        }
    }
    if (iFile == 4 && y == 0 && x == 2){ // Stays on the same rank, is on e file, and would move 2 files left or right
        return king->isCastleLegal(bd, pb, iRank, iFile, fRank, fFile, pTurn); // is castling legal?
    }
    return false;
}
bool King::isCastleLegal(Piece* bd[8][8], Player* pb[2], int iRank, int iFile, int fRank, int fFile, char pTurn){
    Piece* king = bd[iRank][iFile];
    if (king->pieceHasMoved()) return false; // We check up here to optimize the case that the king already moved.
    
    Piece* rook = bd[iRank][7];
    bool kingside = true; // If true, castle kingside. False, castle queenside. We start with true since castling kingside is more common in games.
    
    if (iFile > fFile){ // final file is left of initial file
        rook = bd[iRank][0]; // Castle queenside
        kingside = false;
    }
    if (rook == nullptr || rook->getName() != "Rook" || rook->getColor() != king->getColor()){
        return false; // This means that there is no piece there or it is not a rook of the same color as the king.
    }
    if (rook->pieceHasMoved()){
        return false; // Castling is not legal if the rook has moved.
    }
    
    if (!lineNoObstructions(bd, iRank, iFile, fRank, fFile)){
        return false; // If there are any pieces between the rook and king, castling is illegal.
    }
    
    int pNum = 0;
    switch (pTurn){
        case 'B':
            pNum = 1;
            break;
        default:
            break;
    }
    if (pb[pNum]->checkStatus()){
        return false; // Cannot castle if currently in check.
    }
    if (!king->isValidMove(bd, pb, iRank, iFile, iRank, fFile, pTurn)){
        return false; // Cannot move to dest square if the king will be in check
    }
    if (kingside){
        if (!king->isValidMove(bd, pb, iRank, iFile, iRank, iFile+1, pTurn)){
            return false;
        }
    }
    else {
        if (!king->isValidMove(bd, pb, iRank, iFile, iRank, iFile-1, pTurn)){
            return false;
        }
    }
    return true; // Has passed all tests. In this case, castling is a valid move!
}

bool King::inCheck(Piece* bd[8][8], Player* pb[2], char pTurn, int exemptRank, int exemptFile){
    int kingRow = 0;
    int kingFile = 0;
    // Find king
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (bd[i][j] != 0 && bd[i][j]->getName() == "King" && bd[i][j]->getColor() == pTurn){
                kingRow = i;
                kingFile = j;
            }
        }
    }
    Piece* king = bd[kingRow][kingFile];
    // If an opposing piece has a valid move to the king's square, it is in check.
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (i == exemptRank && j == exemptFile) // This ensures that a piece that would be captured in a move to parry check will not be checked. If this were not here, a legal move could be seen as illegal because the piece would still be in a position to give check.
                continue;
            if (bd[i][j] != 0 && bd[i][j]->getColor() != king->getColor()) {// opposing piece
                // Should not check a piece that would be captured if a move were executed.
                if(bd[i][j]->isSquareValid(bd, pb, pTurn, i, j, kingRow, kingFile)){
                    return true;
                }
            }
        }
    }
    return false;
}
void Piece::promotePawn(Piece* bd[8][8], char newPieceType, int rank, int file){
    // Promotes a pawn when it reaches the end
    char color = bd[rank][file]->getColor();
    switch (newPieceType){
        case 'q':
        case 'Q':
            delete bd[rank][file];
            bd[rank][file] = new Queen(color);
            break;
        case 'r':
        case 'R':
            delete bd[rank][file];
            bd[rank][file] = new Rook(color);
            break;
        case 'n':
        case 'N':
            delete bd[rank][file];
            bd[rank][file] = new Knight(color);
            break;
        case 'b':
        case 'B':
            delete bd[rank][file];
            bd[rank][file] = new Bishop(color);
            break;
        default:
            break;
    }
}


