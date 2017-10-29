#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GameWindow.h"
#include "NetGameWindow.h"
#include "PeopleVsComputer.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(850,640);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAbout_clicked()
{
    QMessageBox::information(this,QStringLiteral("关于"),QStringLiteral("-----本五子棋由aky制作------"),QMessageBox::NoButton);
}

void MainWindow::on_btnComputer_clicked()
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

void MainWindow::on_btnNet_clicked()
{
    this->close();
    NetGameWindow* NGW=new NetGameWindow;
    NGW->show();

}

void MainWindow::on_btnPeople_clicked()
{
    this->close();
    GameWindow* GW=new GameWindow;
    GW->show();
}
