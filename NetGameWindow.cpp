#include "NetGameWindow.h"
#include "ui_NetGameWindow.h"
#include <QPainter>
#include <QMessageBox>
#include <QUdpSocket>
#include <QHostInfo>
#include <QScrollBar>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include "GameWindow.h"
#include "MainWindow.h"
#include "PeopleVsComputer.h"
#include <QFileDialog>
NetGameWindow::NetGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetGameWindow),game()
{
    ui->setupUi(this);
    mouseflag=0;
    firstReady=0;
    myColor=false;
    myReady=0;
    otherReady=0;
    udpSocket=new QUdpSocket(this);
    port = 45454;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    sendMessage(NewParticipant);
    this->setFixedSize(850,720);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);
    centralWidget()->setMouseTracking(true);
    setMouseTracking(true);
}
void NetGameWindow::sendMessage(MessageType type,QString serverAddress)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName=QHostInfo::localHostName();
    QString address=getIP();
    out<<type<<getUserName()<<localHostName;
    switch(type)
    {
        case Message:
            if(ui->messageTextEdit->toPlainText()=="")
            {
                QMessageBox::warning(0,"warning",QStringLiteral("文本不能为空"),QMessageBox::Ok);
                return;
            }
            out<<address<<getMessage();
            ui->messageBrowser->verticalScrollBar()->setValue(ui->messageBrowser->verticalScrollBar()->maximum());
            break;
        case NewParticipant:
            out<<address;
            break;
        case ParticipantLeft:
            break;
        case Ready:
            break;
        case Gamesite:
            out<<getGame();
            break;
        case Back:
            break;
        case returnInt:
            out<<returnint;
            if(returnint==QMessageBox::Yes)
            {
                game.backChess();
                mouseflag-=2;
                update();
            }
            break;
        case Giveup:
            break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);
}

void NetGameWindow::participantLeft(QString userName, QString localHostName, QString time)
{
        int rowNum = ui->userTableWidget->findItems(localHostName, Qt::MatchExactly).first()->row();
        ui->userTableWidget->removeRow(rowNum);
        ui->messageBrowser->setTextColor(Qt::black);
        ui->messageBrowser->setCurrentFont(QFont("Times New Roman", 10));
        ui->messageBrowser->append(QStringLiteral("%1离开%2").arg(userName).arg(time));
}
void NetGameWindow::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        in>>messageType;
        QString userName,localHostName,ipAddress,message;
        QPoint gamesite;
        QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch(messageType)
        {
            case Message:
                in>>userName>>localHostName>>ipAddress>>message;
                ui->messageBrowser->setTextColor(Qt::blue);
                ui->messageBrowser->setCurrentFont(QFont("Times New Roman",12));
                ui->messageBrowser->append("["+localHostName+"]"+time);
                ui->messageBrowser->append(message);
                break;
            case NewParticipant:
                in>>userName>>localHostName>>ipAddress;
                newParticipant(userName,localHostName,ipAddress);
                break;
            case ParticipantLeft:
                in>>userName>>localHostName;
                participantLeft(userName,localHostName,time);
                break;
            case Giveup:
                in>>userName>>localHostName;
                if(myColor)
                  QMessageBox::about(this,QStringLiteral(""),QStringLiteral("黑棋认输,白棋获胜"));
                else
                  QMessageBox::about(this,QStringLiteral(""),QStringLiteral("白棋认输,黑棋获胜"));
                mouseflag=0;
                break;
            case Back:
                in>>userName>>localHostName;
                if(localHostName!=QHostInfo::localHostName())
                {
                    returnint=QMessageBox::question(this,QStringLiteral("悔棋"),QStringLiteral("是否同意悔棋"),QMessageBox::Yes,QMessageBox::No);
                    sendMessage(returnInt);
                }
                break;
            case returnInt:
                in>>userName>>localHostName>>returnint;
                if(localHostName!=QHostInfo::localHostName())
                    if(returnint==QMessageBox::Yes)
                    {
                        game.backChess();
                        mouseflag-=2;
                        update();
                    }
                break;
            case Ready:
                in>>userName>>localHostName;
                firstReady++;
                if(localHostName==QHostInfo::localHostName())
                {
                    myReady=firstReady;
                }
                else
                {
                    otherReady=firstReady;
                }
                ui->messageBrowser->setTextColor(Qt::black);
                ui->messageBrowser->setCurrentFont(QFont("Times New Roman",10));
                ui->messageBrowser->append(QStringLiteral("%1准备").arg(userName));
                if(myReady!=0&&otherReady!=0)
                {
                    if(myReady<otherReady)
                    {

                        mouseflag=1;
                        myColor=true;
                    }
                    else
                    {
                        mouseflag=0;
                        myColor=false;
                    }
                    ui->messageBrowser->append(QStringLiteral("游戏开始！"));
                }
                break;
            case Gamesite:
                in>>userName>>localHostName>>gamesite;
                currentX=gamesite.x();
                currentY=gamesite.y();
                game.peoplePutDown(gamesite.x(),gamesite.y());
                update();
                mouseflag++;
                if(game.ifWin(gamesite.x(),gamesite.y()))
                {
                    if((game.player-1)%2)
                     QMessageBox::about(this,QStringLiteral("恭喜"),QStringLiteral("白棋获胜"));
                    else
                        QMessageBox::about(this,QStringLiteral("恭喜"),QStringLiteral("黑棋获胜"));
                    mouseflag=0;
                }

                update();
                break;
        }
    }
}

void NetGameWindow::on_sendButton_clicked()
{
    sendMessage(Message);

}
void NetGameWindow::newParticipant(QString userName, QString localHostName, QString ipAddress)
{
    bool isEmpty=ui->userTableWidget->findItems(localHostName,Qt::MatchExactly).isEmpty();
    if(isEmpty)
    {
        QTableWidgetItem *user = new QTableWidgetItem(userName);
        QTableWidgetItem *host = new QTableWidgetItem(localHostName);
        QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);
        ui->userTableWidget->verticalHeader()->setVisible(false);
        ui->userTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::Section{background:rgb(178, 113, 60);}");
        ui->userTableWidget->insertRow(0);
        ui->userTableWidget->setItem(0,0,user);
        ui->userTableWidget->setItem(0,1,host);
        ui->userTableWidget->setItem(0,2,ip);
        ui->messageBrowser->setTextColor(Qt::black);
        ui->messageBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->messageBrowser->append(QStringLiteral("%1在线").arg(userName));
        sendMessage(NewParticipant);
    }
}
QString NetGameWindow::getMessage()
{
    QString msg=ui->messageTextEdit->toHtml();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return msg;
}

QPoint NetGameWindow::getGame()
{
    return QPoint(currentX,currentY);
}
QString NetGameWindow::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
       return 0;
}
QString NetGameWindow::getUserName()
{
    QStringList envVariables;
    //???5?string??envVariables????
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    //?????????褏???environment?
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables) {
        //indexOf????????list???,QRegExp`藝?????????????`
        int index = environment.indexOf(QRegExp(string));
        if (index != -1) {
            //stringList???藝environment.at(index)??袞'='??????
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2) {
                return stringList.at(1);//at(0)?蟿?"USERNAME."矛at(1)????
                break;
            }
        }
    }
    return "unknown";
}

NetGameWindow::~NetGameWindow()
{
    delete ui;
}

void NetGameWindow::paintEvent(QPaintEvent *)
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
            painter.drawLine(40+i*40,60,40+i*40,620);//?袩
            painter.drawLine(40,60+i*40,600,60+i*40);//?袩
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
    painter.drawEllipse(650,80,50,50);
    brush.setColor(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawEllipse(750,80,50,50);
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    if((game.player)%2)
    {
       QPoint points[3]={QPoint(765,160),QPoint(775,145),QPoint(785,160)};
       painter.drawPolygon(points,3);
    }
    else
    {
        QPoint points[3]={QPoint(665,160),QPoint(675,145),QPoint(685,160)};
        painter.drawPolygon(points,3);
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
void NetGameWindow::mouseMoveEvent(QMouseEvent *event)
{
    moveX=(event->y()-40)/40;
    moveY=(event->x()-20)/40;
    update();
}

void NetGameWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int x,y;
    if(mouseflag%2)
    {
        x=(event->y()-40)/40;
        y=(event->x()-20)/40;
        if(event->x()>=20&&event->x()<=620&&event->y()>=40&&event->y()<=640)
        {
            if(game.judge(x,y))
            {
                currentX=x;
                currentY=y;
                sendMessage(Gamesite);
                update();
            }
            else
                QMessageBox::information(this,QStringLiteral("注意"),QStringLiteral("已存在棋子，请重下！"),QMessageBox::Ok);
        }
        else
            QMessageBox::information(this,QStringLiteral("注意"),QStringLiteral("不在棋盘内，请重下！"),QMessageBox::Ok);
        update();
    }
}




void NetGameWindow::on_readyBtn_clicked()
{
    sendMessage(Ready);
}
void NetGameWindow::on_giveupBtn_clicked()
{
    int ret=QMessageBox::question(this,QStringLiteral("认输"),QStringLiteral("确定认输?"),QMessageBox::Yes,QMessageBox::No);
    if(ret==QMessageBox::Yes)
    {
        sendMessage(Giveup);

    }
}

void NetGameWindow::on_backBtn_clicked()
{
    int ret=QMessageBox::question(this,QStringLiteral("悔棋"),QStringLiteral("是否请求悔棋"),QMessageBox::Yes,QMessageBox::No);
    if(ret==QMessageBox::Yes)
        sendMessage(Back);
}

void NetGameWindow::on_quitBtn_clicked()
{
    sendMessage(ParticipantLeft);
}

void NetGameWindow::on_actionNet_triggered()
{
    this->close();
    NetGameWindow* NG=new NetGameWindow;
    NG->show();
}

void NetGameWindow::on_actionHome_triggered()
{
    this->close();
    MainWindow* MW=new MainWindow;
    MW->show();
}

void NetGameWindow::on_actionScreenshot_triggered()
{
    QPixmap screenshot=this->grab(QRect(20,40,600,620));
    QString filename = QFileDialog::getSaveFileName(this,QStringLiteral("保存文件"),"", "Image File(*.jpg)");
        if(filename.length()>0)
            screenshot.save(filename);
}

void NetGameWindow::on_actionComputer_triggered()
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

void NetGameWindow::on_actionPeople_triggered()
{

    this->close();
    GameWindow* GW=new GameWindow;
    GW->show();
}

void NetGameWindow::on_actionQuit_triggered()
{
    this->close();
}

void NetGameWindow::on_actionBack_triggered()
{
    int ret=QMessageBox::question(this,QStringLiteral("悔棋"),QStringLiteral("是否确定悔棋"),QMessageBox::Yes,QMessageBox::No);
    if(ret==QMessageBox::Yes)
        sendMessage(Back);
}
