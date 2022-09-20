//
//  Board.cpp
//  Chess Game
//
//  Created by Wade Sultan on 8/16/22.
//

#include <stdio.h>
#include <iostream>
#include "Board.h"
#include "Piece.h"
using namespace std;

Board::Board(){
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            c_board[row][col] = 0;
            l_board[row][col] = 0;
        }
    }
    // Allocate players
    p_board[0] = 0;
    p_board[1] = 0;
    p_board[0] = new Player("White", false, 0);
    p_board[1] = new Player("Black", false, 0);
    
    // Allocating white pieces
    for (int col = 0; col < 8; col++){
        c_board[1][col] = new Pawn('W');
    }
    c_board[0][0] = new Rook('W');
    c_board[0][1] = new Knight('W');
    c_board[0][2] = new Bishop('W');
    c_board[0][3] = new Queen('W');
    c_board[0][4] = new King('W');
    c_board[0][5] = new Bishop('W');
    c_board[0][6] = new Knight('W');
    c_board[0][7] = new Rook('W');
    
    // Allocating black pieces
    for (int col = 0; col < 8; col++){
        c_board[6][col] = new Pawn('B');
    }
    c_board[7][0] = new Rook('B');
    c_board[7][1] = new Knight('B');
    c_board[7][2] = new Bishop('B');
    c_board[7][3] = new Queen('B');
    c_board[7][4] = new King('B');
    c_board[7][5] = new Bishop('B');
    c_board[7][6] = new Knight('B');
    c_board[7][7] = new Rook('B');
    
}
Board::~Board(){
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            delete c_board[row][col];
            c_board[row][col] = 0;
        }
    }
    // De-allocate players
    delete p_board[0];
    delete p_board[1];
    p_board[0] = 0;
    p_board[1] = 0;
    displayMoves();
}
void Board::display(bool displayLegal){
    for (int row = 0; row<15; row++){
        if (row % 2 == 0){
            cout<<(16-row)/2;
            for (int col = 1; col<=15; col++){
                if (col % 2 != 0){
                    if (c_board[(15-row)/2][col/2] != 0){
                        cout<<c_board[(15-row)/2][col/2]->getPiece();
                    }
                    else if (displayLegal && l_board[(15-row)/2][col/2] != 0)
                    {
                        cout<<l_board[(15-row)/2][col/2]->getPiece();
                    }
                    else
                        cout<<" ";
                }
                else {
                    cout<<"|";
                }
            }
            
        }
        else {
            cout<<"--+-+-+-+-+-+-+-";
        }
        cout<<endl;
    }
    cout<<" a|b|c|d|e|f|g|h "<<endl;
}
