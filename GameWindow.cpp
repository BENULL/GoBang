#include "GameWindow.h"
#include "ui_GameWindow.h"
#include <QPainter>
#include <QMessageBox>
#include "NetGameWindow.h"
#include "PeopleVsComputer.h"
#include "MainWindow.h"
#include <QTime>
#include <QTimer>
#include <QFileDialog>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow),game()
{
    ui->setupUi(this);
    mouseflag=true;
    whiteTimes=300;
    blackTimes=300;
    this->setFixedSize(850,660);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);
    centralWidget()->setMouseTracking(true);
    setMouseTracking(true);
    btimerId=new QTimer(this);
    wtimerId=new QTimer(this);
    connect(btimerId,SIGNAL(timeout()),this,SLOT(btimerUpdate()));
    connect(wtimerId,SIGNAL(timeout()),this,SLOT(wtimerUpdate()));
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPen pen=painter.pen();
    pen.setColor(QColor("#8D5822"));
    pen.setWidth(7);
    painter.setPen(pen);
    QBrush brush;
    brush.setColor(QColor("#EEC085"));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(20,40,600,600);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    for(int i=0;i<15;i++)
        {
            painter.drawLine(40+i*40,60,40+i*40,620);//×ÝÏß
            painter.drawLine(40,60+i*40,600,60+i*40);//ºáÏß
        }
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawRect(155,175,10,10);
    painter.drawRect(475,175,10,10);
    painter.drawRect(155,495,10,10);
    painter.drawRect(475,495,10,10);
    painter.drawRect(315,335,10,10);
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            {
                if (game.chess[i][j] == 4)
                {
                    brush.setColor(Qt::black);
                    painter.setBrush(brush);
                    painter.drawEllipse(QPoint((j + 1) * 40,(i + 1) * 40+20),  18, 18);
                }
                else if (game.chess[i][j] == 5)
                {
                    brush.setColor(Qt::white);
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(brush);
                    painter.drawEllipse(QPoint((j + 1) * 40,(i + 1) * 40+20),  18, 18);
                }
            }
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawEllipse(700,120,50,50);
    brush.setColor(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawEllipse(700,400,50,50);
    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);
    if((moveX*40+40)>=20&&(moveX*40+40)<=620&&(moveY*40+20)>=40&&(moveY*40+20)<=640)
    {
        painter.drawLine((moveY+1)*40-20,(moveX+1)*40,(moveY+1)*40-10,(moveX+1)*40);
        painter.drawLine((moveY+1)*40+20,(moveX+1)*40,(moveY+1)*40+10,(moveX+1)*40);
        painter.drawLine((moveY+1)*40-20,(moveX+1)*40+40,(moveY+1)*40-10,(moveX+1)*40+40);
        painter.drawLine((moveY+1)*40+20,(moveX+1)*40+40,(moveY+1)*40+10,(moveX+1)*40+40);
        painter.drawLine((moveY+1)*40-20,(moveX+1)*40,(moveY+1)*40-20,(moveX+1)*40+10);
        painter.drawLine((moveY+1)*40+20,(moveX+1)*40,(moveY+1)*40+20,(moveX+1)*40+10);
        painter.drawLine((moveY+1)*40-20,(moveX+1)*40+40,(moveY+1)*40-20,(moveX+1)*40+30);
        painter.drawLine((moveY+1)*40+20,(moveX+1)*40+40,(moveY+1)*40+20,(moveX+1)*40+30);
    }
        painter.drawLine((currentY+1)*40-1,(currentX+1)*40+20,(currentY+1)*40-6,(currentX+1)*40+20);
        painter.drawLine((currentY+1)*40+1,(currentX+1)*40+20,(currentY+1)*40+6,(currentX+1)*40+20);
        painter.drawLine((currentY+1)*40,(currentX+1)*40+19,(currentY+1)*40,(currentX+1)*40+14);
        painter.drawLine((currentY+1)*40,(currentX+1)*40+21,(currentY+1)*40,(currentX+1)*40+26);
}

void GameWindow::mouseReleaseEvent(QMouseEvent* event)
{
    int x,y;
    if(mouseflag)
    {
        x=(event->y()-40)/40;
        y=(event->x()-20)/40;
        if(event->x()>=20&&event->x()<=620&&event->y()>=40&&event->y()<=640)
        {
            if(game.judge(x,y))
            {
                game.peoplePutDown(x,y);
                if((game.player-1)%2)
                {
                    wtimerId->stop();
                    btimerId->start(1000);
                }
                else
                {
                    btimerId->stop();
                    wtimerId->start(1000);
                }
                currentX=x;
                currentY=y;
                update();
                if(game.ifWin(x,y))
                {
                    if((game.player-1)%2)
                        QMessageBox::about(this,QStringLiteral("¹§Ï²"),QStringLiteral("°×Æå»ñÊ¤"));
                    else
                        QMessageBox::about(this,QStringLiteral("¹§Ï²"),QStringLiteral("ºÚÆå»ñÊ¤"));
                    btimerId->stop();
                    wtimerId->stop();
                    mouseflag=false;
                }
            }
            else
                QMessageBox::information(this,QStringLiteral("×¢Òâ"),QStringLiteral("ÒÑ´æÔÚÆå×Ó£¬ÇëÖØÏÂ£¡"),QMessageBox::Ok);
        }
        else
            QMessageBox::information(this,QStringLiteral("×¢Òâ"),QStringLiteral("²»ÔÚÆåÅÌÄÚ£¬ÇëÖØÏÂ£¡"),QMessageBox::Ok);
        update();
    }
}

void GameWindow::mouseMoveEvent(QMouseEvent *event)
{
    moveX=(event->y()-40)/40;
    moveY=(event->x()-20)/40;
    update();
}
void GameWindow::btimerUpdate()
{
    QString timeString=QString("%1-%2").arg(whiteTimes/60,2,10,QChar('0')).arg(whiteTimes%60,2,10,QChar('0'));
    ui->whiteTime->setText(timeString);
    if(!whiteTimes)
    {
        QMessageBox::about(this,QStringLiteral("¹§Ï²"),QStringLiteral("°×Æå³¬Ê±,ºÚÆå»ñÊ¤!"));
    }
    else
        whiteTimes--;

}
void GameWindow::wtimerUpdate()
{
    QString timeString=QString("%1-%2").arg(blackTimes/60,2,10,QChar('0')).arg(blackTimes%60,2,10,QChar('0'));
    ui->blackTime->setText(timeString);
    if(!blackTimes)
    {
        QMessageBox::about(this,QStringLiteral("¹§Ï²"),QStringLiteral("ºÚÆå³¬Ê±,°×Æå»ñÊ¤!"));
    }
    else
        blackTimes--;
}

void GameWindow::on_actionBack_triggered()
{
    game.backChess();
}

void GameWindow::on_actionHome_triggered()
{
    this->close();
    MainWindow* MW=new MainWindow;
    MW->show();
}

void GameWindow::on_actionNet_triggered()
{

    this->close();
    NetGameWindow* NGW=new NetGameWindow;
    NGW->show();
}

void GameWindow::on_actionQuit_triggered()
{
    this->close();
}

void GameWindow::on_actionPeople_triggered()
{
    game.newChess();
    blackTimes=300;
    whiteTimes=300;
    currentX=0;
    currentY=0;
    mouseflag=true;
}

void GameWindow::on_actionComputer_triggered()
{
    int ret;
    this->close();
    PeopleVsComputer* pvc=new PeopleVsComputer;
    pvc->show();
    ret= pvc->judgeWhoFirst();
    if(ret==QMessageBox::Yes)
        pvc->youFirst();
    else if(ret==QMessageBox::No)
        pvc->computerFirst();
}

void GameWindow::on_actionScreenshot_triggered()
{
    QPixmap screenshot=this->grab(QRect(20,40,600,620));
    QString filename = QFileDialog::getSaveFileName(this,QStringLiteral("±£´æÍ¼Æ¬"),"", "Image File(*.jpg)");
        if(filename.length()>0)
            screenshot.save(filename);
}
