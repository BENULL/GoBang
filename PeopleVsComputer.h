#ifndef PEOPLEVSCOMPUTER_H
#define PEOPLEVSCOMPUTER_H

#include <QMainWindow>
#include <QMouseEvent>
#include "Game.h"
namespace Ui {
class PeopleVsComputer;
}

class PeopleVsComputer : public QMainWindow
{
    Q_OBJECT

public:
    explicit PeopleVsComputer(QWidget *parent = 0);
    ~PeopleVsComputer();
    virtual void paintEvent(QPaintEvent*);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    int judgeWhoFirst();
    void youFirst();
    void computerFirst();
private slots:
    void on_actionBack_triggered();

    void on_actionPeople_triggered();

    void on_actionQuit_2_triggered();

    void on_actionNet_triggered();

    void on_actionHome_triggered();

    void on_actionScreenshot_triggered();

    void on_actionComputer_triggered();

private:
    Ui::PeopleVsComputer *ui;
    Game game;
    int currentX;
    int currentY;
    int moveX;
    int moveY;
    bool mouseflag;
};

#endif // PEOPLEVSCOMPUTER_H
