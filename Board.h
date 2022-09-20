//
//  Board.h
//  Chess Game
//
//  Created by Wade Sultan on 8/12/22.
//

#ifndef Board_h
#define Board_h
#include <vector>
#include <string>
#include "Piece.h"
#include "Player.h"
using namespace std;

// Constants for move type to print legal moves.
const int NORMAL_MOVE = 0;
const int CAPTURE_MOVE = 1;
const int EN_PASSANT = 2;
const int CASTLE = 3;

class Legal {
public:
    Legal(int mType): m_moveType(mType){}
    ~Legal(){}
    char getPiece(){
        switch(m_moveType){
            case EN_PASSANT:
                return 'E';
            case CASTLE:
                return 'C';
            default:
                return '*';
        }
    };
private:
    int m_moveType;
};

class Board {
public:
    Board();
    ~Board();
    void display(bool displayLegal); // Bool displayLegal is for whether or not to display legal moves.
    auto moveCount(){
        return size(sq);
    }
    void addMove(char pType, string dest){
        piece.push_back(pType);
        sq.push_back(dest);
    }
    void deleteMoves(){
        piece.clear();
        sq.clear();
    }
    void addLegalMove(int moveType, Piece* bd[8][8], int row, int col, string sq){
        switch (moveType){
            case CAPTURE_MOVE:
                pieceName.push_back(bd[row][col]->getName());
                square.push_back(sq);
                break;
            case EN_PASSANT:
                l_board[row][col] = new Legal(EN_PASSANT);
                break;
            case CASTLE:
                l_board[row][col] = new Legal(CASTLE);
                break;
            default:
                l_board[row][col] = new Legal(NORMAL_MOVE);
                break;
        }
    }
    void displayCapMoves(){
        cout<<"Pieces you can capture:"<<endl;
        for (int i = 0; i<pieceName.size(); i++){ // pieceName and square will always have the same size
            cout<<pieceName[i]<<" on "<<square[i]<<endl;
        }
    }
    bool existLegalCaps(){
        return !square.empty();
    }
    void clearLegalMoves(Legal* lb[8][8]){
        pieceName.clear();
        square.clear();
        for (int i = 0; i<8; i++){
            for (int j = 0; j<8; j++){
                if (lb[i][j] != 0){
                    delete l_board[i][j];
                    lb[i][j] = 0;
                }
            }
        }
    }
    void displayMoves(){
        cout<<"Move history:"<<endl;
        cout<<"White: Black:"<<endl;
        for (int i = 0; i<size(piece); i++){
            cout<<piece[i]<<" "<<sq[i]<<"   ";
            if (i % 2 != 0)
                cout<<endl;
        }
    }
    Piece* c_board[8][8];
    Legal* l_board[8][8];
    Player* p_board[2];
private:
    char m_turn;
    vector<string> pieceName;
    vector<string> square;
    
    // Move history vectors
    vector<char> piece;
    vector<string> sq;
};


#endif /* Board_h */
