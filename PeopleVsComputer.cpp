#include "PeopleVsComputer.h"
#include "ui_PeopleVsComputer.h"
#include "QMessageBox"
#include <QPainter>
#include "NetGameWindow.h"
#include <QFileDialog>
#include "GameWindow.h"
#include "MainWindow.h"
PeopleVsComputer::PeopleVsComputer(QWidget *parent) :
    QMainWindow(parent),game(),
    ui(new Ui::PeopleVsComputer)
{
    ui->setupUi(this);  
    this->setFixedSize(660,660);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);
    mouseflag=false;
    centralWidget()->setMouseTracking(true);
    setMouseTracking(true);
}
int PeopleVsComputer::judgeWhoFirst()
{
    QMessageBox box(QMessageBox::Question,QStringLiteral("选择"),QStringLiteral("由谁先手"));
    box.setStandardButtons (QMessageBox::Yes|QMessageBox::No);
    box.setButtonText (QMessageBox::Yes,QString(QStringLiteral("电脑后手")));
    box.setButtonText (QMessageBox::No,QString(QStringLiteral("电脑先手")));
    return box.exec();
}

void PeopleVsComputer::youFirst()
{
    game.computerColor=5;
    mouseflag=true;
}

void PeopleVsComputer::computerFirst()
{
    game.computerColor=4;
    game.computerPutdown();
    mouseflag=true;
}

void PeopleVsComputer::mouseReleaseEvent(QMouseEvent* event)
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
                currentX=x;
                currentY=y;
                update();
                if(game.ifWin(x,y))
                {
                    if((game.player-1)%2)
                        QMessageBox::about(this,QStringLiteral("恭喜"),QStringLiteral("白棋获胜"));
                    else
                        QMessageBox::about(this,QStringLiteral("恭喜"),QStringLiteral("黑棋获胜"));
                    mouseflag=false;
                }
                mouseflag=false;
                QPoint point=game.computerPutdown();
                currentX=point.x();
                currentY=point.y();
                update();
                mouseflag=true;
                if(game.ifWin(currentX,currentY))
                {
                    if((game.player-1)%2)
                        QMessageBox::about(this,QStringLiteral("恭喜"),QStringLiteral("白棋获胜"));
                    else
                        QMessageBox::about(this,QStringLiteral("恭喜"),QStringLiteral("黑棋获胜"));
                    mouseflag=false;
                }


            }
            else
                QMessageBox::information(this,QStringLiteral("注意"),QStringLiteral("已存在棋子，请重下！"),QMessageBox::Ok);
        }
        else
            QMessageBox::information(this,QStringLiteral("注意"),QStringLiteral("不在棋盘内，请重下！"),QMessageBox::Ok);
        update();
    }
}
void PeopleVsComputer::paintEvent(QPaintEvent *)
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
            painter.drawLine(40+i*40,60,40+i*40,620);//纵线
            painter.drawLine(40,60+i*40,600,60+i*40);//横线
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
void PeopleVsComputer::mouseMoveEvent(QMouseEvent *event)
{
    moveX=(event->y()-40)/40;
    moveY=(event->x()-20)/40;
    update();
}
PeopleVsComputer::~PeopleVsComputer()
{
    delete ui;
}

void PeopleVsComputer::on_actionBack_triggered()
{
    game.backChess();
}

void PeopleVsComputer::on_actionPeople_triggered()
{

    this->close();
    GameWindow* GW=new GameWindow;
    GW->show();
}

void PeopleVsComputer::on_actionQuit_2_triggered()
{
    this->close();
}

void PeopleVsComputer::on_actionNet_triggered()
{
    this->close();
    NetGameWindow* NGW=new NetGameWindow;
    NGW->show();
}

void PeopleVsComputer::on_actionHome_triggered()
{
    this->close();
    MainWindow* MW=new MainWindow;
    MW->show();
}

void PeopleVsComputer::on_actionScreenshot_triggered()
{
    QPixmap screenshot=this->grab(QRect(20,40,600,620));
    QString filename = QFileDialog::getSaveFileName(this,QStringLiteral("保存图片"),"", "Image File(*.jpg)");
        if(filename.length()>0)
            screenshot.save(filename);
}

void PeopleVsComputer::on_actionComputer_triggered()
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
