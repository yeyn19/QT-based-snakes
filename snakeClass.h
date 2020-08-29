#ifndef SNAKECLASS_H
#define SNAKECLASS_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <QtWidgets>


using namespace std;



class maps{
public:
    int ifWall = 0;
    int ifFood = 0;
    int ifSnake = 0;
    maps(int w = 0, int f = 0, int s = 0): ifWall(w), ifFood(f), ifSnake(s){};
};

class Snake{
public:
    int len;
    int direx;
    int direy;
    int ifDeath;
    int ifSpeed;
    int ifGrow;
    int speedTime;
    vector<pair<int,int>> body;


    ~Snake(){
        body.clear();
    }
    Snake(pair<int,int> head, pair<int,int> tail){
        //int det = abs(head.first - tail.first) + abs(head.second - tail.second);
        //assert(det == 1);
        body.push_back(tail);
        body.push_back(head);
        direx = head.first - tail.first;
        direy = head.second - tail.second;
        ifGrow = 0;
        len = 2;
        ifDeath = 0;
        ifSpeed = 0;

    }

    Snake(int ll, int G, int D,vector<pair<int,int>> bb): len(ll), body(bb), ifGrow(G), ifDeath(D){
        pair<int,int> hh = bb[ll-1];
        pair<int,int> next = bb[ll-2];
        direx = hh.first - next.first;
        direy = hh.second - next.second;
        ifSpeed = 0;
        speedTime = 0;
    }

    void setDire(char de){
        pair<int,int> hh = body[len-1];
        pair<int,int> next = body[len-2];
        int nx = hh.first - next.first;
        int ny = hh.second - next.second;

        if (de == 'w'){
            if (direx == 0 && direy == -1) {
                ifSpeed = 1;
                return;
            }
            if (nx == 0) return;

            direx = 0;
            direy = -1;
        }
        else if (de == 's'){
            if (direx == 0 && direy == 1) {
                ifSpeed = 1;
                return;
            }
            if (nx == 0) return;
            direx = 0;
            direy = 1;
        }
        else if (de == 'a'){
            if (direx == -1 && direy == 0) {
                ifSpeed = 1;
                return;
            }
            if (ny == 0) return;
            direx = -1;
            direy = 0;
        }
        else{
            if (direx == 1 && direy == 0) {
                ifSpeed = 1;
                return;
            }
            if (ny == 0) return;
            direx = 1;
            direy = 0;
        }
    }
    public slots:
    void init(maps** mmp){
        if (ifDeath) return;
        if (ifSpeed) speedTime++;
        pair<int,int> head = body[len-1];
        pair<int,int> news = make_pair(head.first+direx,head.second+direy);
        body.push_back(news);
        if (mmp[head.first][head.second].ifFood != 0){
            ifGrow += 3;
            mmp[head.first][head.second].ifFood = 0;

        }
        if (mmp[news.first][news.second].ifWall == 1 || mmp[news.first][news.second].ifSnake == 1){
            ifDeath = 1;
        }



        if(ifGrow > 0) {
            ifGrow--;
            len++;
        }
        else{
            body.erase(body.begin());
        }

        if (len >= 6 && speedTime % 10 == 1){
            qsrand(QTime::currentTime().msec());
            int x = body[0].first;
            int y = body[0].second;
            body.erase(body.begin());
            mmp[x][y].ifFood = 1 + qrand() % 5;
            body.erase(body.begin());
            body.erase(body.begin());
            len -= 3;
            speedTime = 2;

        }
    }
    pair<int,int> head(){
        return body[len-1];
    }
signals:
    void growth();
};



#endif // SNAKECLASS_H
