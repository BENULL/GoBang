#ifndef GAME_H
#define GAME_H
#include <QVector>
typedef struct
{
  int x,y,l,r;
}value;
class Game
{
public:
    Game();
    int chess[15][15];
    int player;
    bool win1(int x,int y);
    bool win2(int x,int y);
    bool win3(int x,int y);
    bool win4(int x,int y);
    bool judge(int x,int y);
    bool ifWin(int x,int y);
    void peoplePutDown(int x,int y);
    void backChess();
    void newChess();
    void computerBackChess();
    void setFiveValue();
    int chart(int count);
    void getSiteValue();
    int computerColor;
    QPoint computerPutdown();
private:
    QVector<QPoint> chessPoint;
    value fiveValue[15][15];
    int siteValue[15][15];
};

#endif // GAME_H
