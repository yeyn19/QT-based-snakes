#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTimer>
#include "snakeClass.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class Dialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr, int c = 1, int t = 0, double speed = 50);
    void paintEvent(QPaintEvent* ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void setButton(int stt);

    void randFood(int foods);
    void randSnake();
    void createMap();
    void initMap();
    void initGame();
    void concon();
    void pos2pix(int x, int y, int* zi);
    void pix2pos(int x, int y, int *zi);
    void reset(int pls = 1);
    void save(QString pos);
    void load(QString pos);

    int height = 40;
    int width = 40;
    int startX = 190;
    int startY = 130;
    int pix = 15;

    ~MainWindow();



private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_actionpause_triggered();

    void on_actionsave_triggered();

    void on_actionload_triggered();


    void on_actionquit_triggered();

    void on_actionrestart_triggered();

    void on_actionstart_triggered();

    void on_actioncontinue_triggered();

private:
    int* color;
    int times;
    int st = 0;
    int addM = 0;
    double fps;
    //QDialog* dia;
    Ui::MainWindow *ui;
    int snakeC;
    maps** mms;
    QToolBar* qq;
    QTimer* qtime;
    QAction* startA;
    QAction* pauseA;
    QAction* saveA;
    QAction* loadA;
    QAction* play1A;
    QAction* play2A;
    QAction* exitA;

    vector<Snake> ss;

};
#endif // MAINWINDOW_H
