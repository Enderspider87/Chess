//
//  Player.h
//  Chess Game
//
//  Created by Wade Sultan on 8/12/22.
//

#ifndef Player_h
#define Player_h
#include <iostream>
#include <string>
using namespace std;

class Player {
public:
    Player(string color, bool checkStat, int moveCount): m_color(color), inCheck(checkStat), m_count(moveCount){}
    ~Player(){}
    string getPlayerColor(){
        return m_color;
    }
    bool checkStatus(){
        return inCheck;
    }
    void intoCheck(){
        inCheck = true;
    }
    void outOfCheck(){
        inCheck = false;
    }
    int getMoveCount(){
        return m_count;
    }
    void incMoveCount(){ // Increment move count for the player
        m_count++;
    }
private:
    string m_color;
    bool inCheck;
    int m_count;
};


#endif /* Player_h */
