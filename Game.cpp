#include "Game.h"
#include <memory.h>

Game::Game()
{
    memset(chess,0,sizeof(chess));
    memset(fiveValue,0,sizeof(fiveValue));
    player=0;
}
bool Game::judge(int x,int y)
{
    if(!chess[x][y])
        return true;
    else
        return false;
}
void Game::peoplePutDown(int x,int y)
{
    chess[x][y]=player++%2+4;
    chessPoint.push_back(QPoint(x,y));
}
QPoint Game::computerPutdown()
{
    int temp=0,x=0,y=0;
    setFiveValue();
    getSiteValue();
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
            {
                if(chess[i][j]==0)
                    if(siteValue[i][j]>=temp)
                    {
                        temp=siteValue[i][j];
                        x=i;
                        y=j;
                    }

            }

    if(player==0)
    {
        chess[7][7]=player++%2+4;
        chessPoint.push_back(QPoint(7,7));
    }
    else
    {
        chess[x][y]=player++%2+4;
        chessPoint.push_back(QPoint(x,y));
    }

    return QPoint(x,y);
}
bool Game::ifWin(int x, int y)
{
    if(win1(x,y)||win2(x,y)||win3(x,y)||win4(x,y))
        return true;
    else
        return false;
}
void Game::backChess()
{
    int x,y;
    for(int i=0;i<2;i++)
    {
        x=chessPoint.last().x();
        y=chessPoint.last().y();
        chess[x][y]=0;
        player--;
        chessPoint.pop_back();
    }
}
void Game::newChess()
{
    memset(chess,0,sizeof(chess));
    player=0;
    chessPoint.clear();
}
void Game::setFiveValue()
{
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
        {
            int count=0;          //x
            if(j<11)
            {
                for(int n=0;n<5;n++)
                {
                  count+=chess[i][j+n];
                }
                fiveValue[i][j].x=chart(count);
            }
            count=0;             //y
            if(i<11)
            {
                for(int n=0;n<5;n++)
                {
                  count+=chess[i+n][j];
                }
                fiveValue[i][j].y=chart(count);
            }
            count=0;             //r
            if(i<11&&j<11)
            {
                for(int n=0;n<5;n++)
                {
                  count+=chess[i+n][j+n];
                }
                fiveValue[i][j].r=chart(count);
            }
            count=0;             //l
            if(j>=4&&i<11)
            {
                for(int n=0;n<5;n++)
                {
                  count+=chess[i+n][j-n];
                }
                fiveValue[i][j].l=chart(count);
            }
        }
}
void Game::getSiteValue()
{
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
        {
            siteValue[i][j]=fiveValue[i][j].l+fiveValue[i][j].r+fiveValue[i][j].x+fiveValue[i][j].y;
            for(int n=1;n<5;n++)
            {
                if(j-n>=0)
                    siteValue[i][j]+=fiveValue[i][j-n].x;
                if(i-n>=0)
                    siteValue[i][j]+=fiveValue[i-n][j].y;
                if(i-n>=0&&j-n>=0)
                    siteValue[i][j]+=fiveValue[i-n][j-n].r;
                if(i-n>=0&&j+n<=14)
                    siteValue[i][j]+=fiveValue[i-n][j+n].l;
            }
        }
}

int Game::chart(int count)
{
    int temp;
    if(computerColor==4)
    {
        switch(count)
        {
           case 0: temp=7;break;
           case 4: temp=35;break;
           case 8: temp=800;break;
           case 12: temp=15000;break;
           case 16: temp=800000;break;
           case 5: temp=15;break;
           case 10: temp=400;break;
           case 15: temp=1800;break;
           case 20: temp=100000;break;
           default: temp=0;
        }
     }
    else if(computerColor==5)
    {
        switch(count)
        {
           case 0: temp=7;break;
           case 4: temp=15;break;
           case 8: temp=400;break;
           case 12: temp=1800;break;
           case 16: temp=100000;break;
           case 5: temp=35;break;
           case 10: temp=800;break;
           case 15: temp=15000;break;
           case 20: temp=800000;break;
           default: temp=0;
        }
    }
    return temp;
}
bool Game::win1(int x,int y)
{
    /*直线左右*/
    int i,count=0;
    for(i=1;i<5;i++)
    {
        if(y-i>=0&&chess[x][y]==chess[x][y-i])
            count++;
        else break;
    }
    for(i=1;i<5;i++)
    {
        if(y+i<=14&&chess[x][y]==chess[x][y+i])
            count++;
        else break;
    }
    if(count>=4)
        return true;
    else
        return false;
}
bool Game::win2(int x,int y)
{
    /***直线上下***/
    int i,count=0;
    for(i=1;i<5;i++)
    {
        if(x-i>=0&&chess[x][y]==chess[x-i][y])
            count++;
        else break;
    }
    for(i=1;i<5;i++)
    {
        if(x+i<=14&&chess[x][y]==chess[x+i][y])
            count++;
        else break;
    }
    if(count>=4)
        return true;
    else
        return false;
}
bool Game::win3(int x,int y)
{
    /***左斜线***/
    int i,count=0;
    for(i=1;i<5;i++)
    {
        if(x-i>=0&&y-i>=0&&chess[x][y]==chess[x-i][y-i])
            count++;
        else break;
    }
    for(i=1;i<5;i++)
    {
        if(y+i<=14&&x+i<=14&&chess[x][y]==chess[x+i][y+i])
            count++;
        else break;
    }
    if(count>=4)
        return true;
    else
        return false;
}
bool Game::win4(int x,int y)
{
        /***右斜线***/
    int i,count=0;
    for(i=1;i<5;i++)
    {
        if(x+i<=14&&y-i>=0&&chess[x][y]==chess[x+i][y-i])
            count++;
        else break;
    }
    for(i=1;i<5;i++)
    {
        if(y+i<=14&&x-i>=0&&chess[x][y]==chess[x-i][y+i])
            count++;
        else break;
    }
    if(count>=4)
        return true;
    else
        return false;
}

