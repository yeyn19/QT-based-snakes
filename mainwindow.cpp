#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "snakeClass.h"
#include <QPainter>
#include <QWidget>
#include <cstdlib>
#include <QRandomGenerator>
#include <time.h>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <cstdio>
#include <stdio.h>
#include <QJsonDocument>
#include "dialog.h"




void MainWindow::randFood(int foods){
    qsrand(QTime::currentTime().msec());
    int x,y;
    for(int i = 0; i < foods; i++){
        do{
            x = 3 + qrand() % (width - 5);
            y = 3 + qrand() % (height - 5);
        }
        while(mms[x][y].ifSnake == 1 || mms[x][y].ifWall == 1 || mms[x][y].ifFood != 0);
        mms[x][y].ifFood = 1 + qrand() % 5;
    }
    if (foods > 0 && snakeC == 1){
        fps = fps * 0.99;
        qtime->setInterval((int)fps);
    }
}

void MainWindow::randSnake(){
    int x,y;
    for(int i = 0 ; i < snakeC ; i++){
        do{
            x = 5 + qrand() % (width - 10);
            y = 5 + qrand() % (height - 10);
        }
        while(mms[x][y].ifSnake == 1 || mms[x][y].ifWall == 1 || mms[x][y].ifFood != 0
              || mms[x][y+1].ifSnake == 1 || mms[x][y+1].ifWall == 1 || mms[x][y+1].ifFood != 0);
        auto h = make_pair(x,y);
        auto t = make_pair(x,y-1);
        Snake k = Snake(t,h);
        ss.push_back(std::move(k));
    }
}


void MainWindow::createMap()
{
    mms = new maps*[100];
    for(int i = 0; i < 100; i++){
        mms[i] = new maps[100];
    }
    for(int i = 0 ; i < 40 ; i++){
        mms[0][i] = maps(1,0,0);
        mms[39][i] = maps(1,0,0);
        mms[i][0] = maps(1,0,0);
        mms[i][39] = maps(1,0,0);
    }
}

void MainWindow::initMap(){
    for(int i = 0 ; i < 2 * height; i++){
        for(int j = 0; j < 2 * width; j++){
            mms[i][j].ifSnake = 0;
        }
    }
    for(auto &i: ss){
        if (i.ifDeath == 1) continue;
        for(auto j : i.body){
            int x = j.first;
            int y = j.second;
            mms[x][y].ifSnake = 1;
        }
    }

    int food = 0;
    for(int i = 0 ; i < height; i ++){
        for(int j = 0; j < width; j++){
            if (mms[i][j].ifFood != 0) food++;
        }
    }
    randFood(snakeC-food);
}

void MainWindow::initGame(){
    createMap();
    initMap();
    randSnake();
    //randFood(snakeC);
    repaint();
}


void MainWindow::concon(){
    times++;

    bool pan = 1;
    for(auto i : ss){
        pan = pan && i.ifDeath;
    }
    if (pan == 1){
        st = 2;
        setButton(2);
        qtime->stop();
        QMessageBox:: StandardButton result= QMessageBox::information(NULL, "Title", "YOU DIED! \n wanna try again?",QMessageBox::Reset|QMessageBox::No);
        switch (result)
        {
        case QMessageBox::Reset:
            reset(1);
            break;
        case QMessageBox::No:
            close();
            break;
        default:
            break;
        }

    }

    for(auto &snake : ss){
        if (snake.ifSpeed || (!snake.ifSpeed && (times % 2 == 0)))
        snake.init(mms);
    }

    initMap();
    repaint();

    if (snakeC > 1 && times % 25 == 0){
        randFood(1);
    }


}


void MainWindow::pos2pix(int x, int y, int *zi){
    zi[0] = x * pix + 1;
    zi[1] = y * pix + 1;
}

void MainWindow::pix2pos(int x, int y, int *zi){
    zi[0] = (x - startX)/ pix;
    zi[1] = (y - startY) / pix;
}

void MainWindow::setButton(int stt){
    if (stt == -1 || stt == 0) ui->pushButton->setEnabled(true);
    else ui->pushButton->setEnabled(false);
    if (stt == -1 || stt == 0) startA->setEnabled(true);
    else startA->setEnabled(false);
    if (stt == -1 || stt == 0) ui->actionstart->setEnabled(true);
    else ui->actionstart->setEnabled(false);

    if (stt == -1 || stt == 0) ui->pushButton_2->setEnabled(true);
    else ui->pushButton_2->setEnabled(false);
    if (stt == -1 || stt == 0) ui->actionrestart->setEnabled(true);
    else ui->actionrestart->setEnabled(false);

    if (stt == -1 || stt == 0) ui->pushButton_3->setEnabled(true);
    else ui->pushButton_3->setEnabled(false);

    if (stt == -1 || stt == 0) play1A->setEnabled(true);
    else play1A->setEnabled(false);

    if (stt == -1 || stt == 0) play2A->setEnabled(true);
    else play2A->setEnabled(false);

    if (stt == 0) ui->pushButton_5->setEnabled(true);
    else ui->pushButton_5->setEnabled(false);

    if (stt == -1) ui->pushButton_6->setEnabled(true);
    else ui->pushButton_6->setEnabled(false);
    if (stt == 0) ui->actionsave->setEnabled(true);
    else ui->actionsave->setEnabled(false);

    if ( stt == 0) saveA->setEnabled(true);
    else saveA->setEnabled(false);

    if (stt == -1) loadA->setEnabled(true);
    else loadA->setEnabled(false);
    if (stt == -1) ui->actionload->setEnabled(true);
    else ui->actionload->setEnabled(false);

    if (stt == 1) ui->pushButton_7->setEnabled(true);
    else ui->pushButton_7->setEnabled(false);
    if (stt == 1) pauseA->setEnabled(true);
    else pauseA->setEnabled(false);
    if (stt == 1) ui->actionpause->setEnabled(true);
    else ui->actionpause->setEnabled(false);

    if (stt == 0) ui->actioncontinue->setEnabled(true);
    else ui->actioncontinue->setEnabled(false);
}


void MainWindow::paintEvent(QPaintEvent* qq){
    QPainter p(this);
    QPainter::Antialiasing;
    //p.translate(startX,startY);
    p.setPen(QPen(QColor(129, 199, 132)));
    QBrush brush(QColor(200, 230, 201), Qt::SolidPattern);
    p.setBrush(brush);
    p.drawRect(0,0, 2 * pix*width, 2 * pix*height);

    p.translate(startX,startY);
    brush.setColor(QColor(129, 199, 132));
    p.setBrush(brush);
    p.drawRect(0,0, pix*width, pix*height);
    brush.setColor(QColor(0,50,255));
    p.setBrush(brush);

    //p.drawLine(QPoint(0, 0), QPoint(100, 100));

    if (st == -1) ui->label->setText(QString("PREPARE"));
    if (st == 0) ui->label->setText(QString("SUSPEND"));
    if (st == 1) ui->label->setText(QString("PLAYING"));
    if (st == 2) ui->label->setText(QString("YOU DIED"));

    string step = "Now Steps: "+ to_string(times);
    ui->label_2->setText(QString::fromStdString(step));

    string speed = " FPS: " + to_string(1000/fps);
    ui->label_3->setText(QString::fromStdString(speed));

    string length = " len-1: " + to_string(ss[0].len);
    if (ss.size() > 1) length += "\n len-2: "+ to_string(ss[1].len);
    ui->label_4->setText(QString::fromStdString(length));

    // print map
    int *k = new int[4];
    brush.setColor(QColor(46, 125, 50));
    p.setBrush(brush);
    for(int i = 0 ; i < height; i++){
        for(int j = 0; j < width; j++){
            if (mms[i][j].ifWall == 1) {
                pos2pix(i,j,k);
                p.drawRoundRect(k[0],k[1],pix-2,pix-2,75,75);
            }
        }
    }
    int ccc[]={0,255,255,0,255,0,255,0,0,251,246,150,0,0,0};
    //226,151,240,251,246,150,152,211,215

    for(int i = 0 ; i < height; i++){
        for(int j = 0; j < width; j++){
            if (mms[i][j].ifFood != 0) {
                int colors = mms[i][j].ifFood;
                QBrush bb(QColor(ccc[3*colors-3], ccc[3*colors-2],ccc[3*colors-1]), Qt::SolidPattern);
                p.setBrush(bb);
                pos2pix(i,j,k);
                p.drawRoundRect(k[0],k[1],pix-2,pix-2,50,50);
            }
        }
    }



    /*pos2pix(0,0,k);
    p.drawRect(k[0],k[1],pix,pix);
    pos2pix(1,1,k);
    p.drawRect(k[0],k[1],pix,pix);
    pos2pix(2,2,k);
    p.drawRect(k[0],k[1],pix,pix);

    brush.setColor(QColor(0,0,0));
    p.setBrush(brush);*/



    //paint snakes
    int cc = 0;
    int x[] = {255,255,255,100,255,218,179, 229, 252,24, 255, 255};
    for(auto &i : ss){
        if (i.ifDeath == 1){
             cc += 6;
                continue;
        }
        brush.setColor(QColor(x[cc],x[cc+1],x[cc+2],100));
        p.setBrush(brush);
        int tou = 255;
        for(auto ge = i.body.rbegin(); ge != i.body.rend(); ge++){
            auto bo = *ge;
            brush.setColor(QColor(x[cc],x[cc+1],x[cc+2],tou-2));
            if (tou > 100) tou -= 2;

            p.setBrush(brush);
            pos2pix(bo.first,bo.second,k);
            p.drawRoundRect(k[0],k[1],pix-2,pix-2,75,75);
        }
        auto head = i.head();
        brush.setColor(QColor(x[cc+3],x[cc+4],x[cc+5]));
        p.setBrush(brush);
        pos2pix(head.first,head.second,k);
        p.drawRoundRect(k[0],k[1],pix-2,pix-2,75,75);
        cc += 6;
    }



    delete [] k;
}


void MainWindow::reset(int pls) {
    ss.clear();
    snakeC = pls;
    qtime->stop();
    times = 0;
    fps = 50;
    qtime->setInterval(fps);
    st = -1;
    setButton(-1);
    initGame();
    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key())
    {
    case Qt::Key_Up:
        ss[0].setDire('w');
        break;
    case Qt::Key_Right:
        ss[0].setDire('d');
        break;
    case Qt::Key_Down:
        ss[0].setDire('s');
        break;
    case Qt::Key_Left:
        ss[0].setDire('a');
        break;
    case Qt::Key_A:
        if (ss.size() > 1)
            ss[1].setDire('a');
        break;
    case Qt::Key_W:
        if (ss.size() > 1)
            ss[1].setDire('w');
        break;
    case Qt::Key_S:
        if (ss.size() > 1)
            ss[1].setDire('s');
        break;
    case Qt::Key_D:
        if (ss.size() > 1)
            ss[1].setDire('d');
        break;
    case Qt::Key_R:
        reset(1);
        break;
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
    case Qt::Key_Space:
        if (st == 1){
                st = 0;
                setButton(0);
                qtime->stop();
        }
        else if (st == 0 || st == -1) {
            qtime->start();
            st = 1;
            setButton(1);
        }
        repaint();
        break;
    default:
        QWidget::keyPressEvent(ev);
    }
}


void MainWindow::keyReleaseEvent(QKeyEvent *ev) {
    switch(ev->key())
    {
    case Qt::Key_Up:
        ss[0].ifSpeed = 0;
        break;
    case Qt::Key_Right:
        ss[0].ifSpeed = 0;
        break;
    case Qt::Key_Down:
        ss[0].ifSpeed = 0;
        break;
    case Qt::Key_Left:
        ss[0].ifSpeed = 0;
        break;
    case Qt::Key_A:
        if (ss.size() > 1)
            ss[1].ifSpeed = 0;
        break;
    case Qt::Key_W:
        if (ss.size() > 1)
            ss[1].ifSpeed = 0;
        break;
    case Qt::Key_S:
        if (ss.size() > 1)
            ss[1].ifSpeed = 0;
        break;
    case Qt::Key_D:
        if (ss.size() > 1)
            ss[1].ifSpeed = 0;
        break;
    default:
        QWidget::keyReleaseEvent(ev);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev) {
    if (st == 1 || st == 2) return;
    int x = ev->pos().x();
    int y = ev->pos().y();
    if (x < startX || y < startY) return;
    int* k = new int[2];
    pix2pos(x,y,k);
    if (mms[k[0]][k[1]].ifFood != 1 && mms[k[0]][k[1]].ifSnake != 1){
        addM = 1- mms[k[0]][k[1]].ifWall;
        mms[k[0]][k[1]].ifWall = 1 - mms[k[0]][k[1]].ifWall;
    }
    repaint();
    delete []k;

}

void MainWindow::mouseMoveEvent(QMouseEvent *ev){
    if (st == 1) return;
    int x = ev->pos().x();
    int y = ev->pos().y();
    int* k = new int[2];
    pix2pos(x,y,k);
    if (mms[k[0]][k[1]].ifFood != 1 && mms[k[0]][k[1]].ifSnake != 1){
        mms[k[0]][k[1]].ifWall = addM;
    }
    repaint();
    delete [] k;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev){
    addM = -1;
}

void MainWindow::load(QString add){

    st = -1;
    setButton(-1);
    qtime->stop();
    ss.clear();

    QByteArray ba2;
    ba2.append(add);     //也可以 ba2 = s2.toLatin1();
    const char *c2 = ba2.data();

    FILE *pf = fopen(c2, "r");
    QFile file;
    file.open(pf,QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    QJsonParseError e;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &e);
    QJsonObject ob = jsonDoc.object();

    //base
    QJsonValue base = ob.value("base");
    QJsonObject baseO = base.toObject();
    QJsonValue ffps = baseO.value("fps");
    fps = ffps.toDouble();
    QJsonValue sn = baseO.value("snakeC");
    snakeC = sn.toInt();
    //qDebug() << snakeC;
    QJsonValue time = baseO.value("time");
    times = time.toInt();
    //qDebug() << times;
    //QJsonObject va = array.at(0);
    //QJsonValue ff = va.value("fps");

    //maps
    createMap();

    QJsonValue maps = ob.value("foods");
    QJsonArray fo = maps.toArray();
    for(int i = 0 ;i < fo.size(); i++){
        QJsonValue food = fo.at(i);
        QJsonObject k = food.toObject();
        int x = (k.value("foodx")).toInt();
        int y = (k.value("foody")).toInt();
        mms[x][y].ifFood = 1;

    }

    //snake
    QJsonValue snakes = ob.value("snakes");
    QJsonArray sss = snakes.toArray();
    for(int i = 0 ;i < sss.size(); i++){
        QJsonValue snake = sss.at(i);
        QJsonObject k = snake.toObject();
        QJsonObject base = k.value("base").toObject();
        int ifD = base.value("ifD").toInt();
        int ifG = base.value("ifG").toInt();
        int len = base.value("len").toInt();
        //qDebug() << ifD;
        //qDebug() << ifG;
        //qDebug() << len;

        QJsonArray bo = k.value("body").toArray();
        vector<pair<int,int>> body;
        for(int i = 0; i < bo.size(); i++){
            QJsonObject pos = bo.at(i).toObject();
            int x = pos.value("x").toInt();
            int y = pos.value("y").toInt();
            body.push_back(make_pair(x,y));
        }
        Snake s = Snake(len,ifG,ifD,body);
        ss.push_back(s);

    }

    initMap();
    repaint();
    qtime->setInterval((int)fps);


    file.close();
}


void MainWindow::save(QString add){
    QJsonObject obj1;
    QJsonObject array;
    obj1.insert("fps",fps);
    obj1.insert("snakeC",snakeC);
    obj1.insert("time",times);
    array.insert("base",obj1);
    QJsonArray fo;

    for(int i = 0 ; i < 100; i++){
        for(int j = 0 ; j < 100; j++){
            if (mms[i][j].ifFood == 1){
                QJsonObject kk;
                kk.insert("foodx",i);
                kk.insert("foody",j);
                fo.push_back(kk);
            }
        }
    }
    array.insert("foods",fo);

    QJsonArray snakes;
    for( auto i : ss){
        QJsonObject arrayS;
        QJsonObject objS;
        objS.insert("len",i.len);
        objS.insert("ifD",i.ifDeath);
        objS.insert("dx",i.direx);
        objS.insert("dy",i.direy);
        objS.insert("ifG",i.ifGrow);
        arrayS.insert("base",objS);
        QJsonArray arrayB;
        for(auto bo: i.body){
            QJsonObject kk;
            kk.insert("x",bo.first);
            kk.insert("y",bo.second);
            arrayB.push_back(kk);

        }
        arrayS.insert("body",arrayB);
        snakes.push_back(arrayS);
    }
    array.insert("snakes",snakes);

    QJsonDocument jsonDoc(array);
    QByteArray ba = jsonDoc.toJson();
    //std::ofstream out("re.json");

    QByteArray ba2;
    ba2.append(add);     //也可以 ba2 = s2.toLatin1();
    const char *c2 = ba2.data();

    FILE *pf = fopen(c2, "w");
    QFile file;
    file.open(pf,QIODevice::WriteOnly);
    //if(!file.open(QIODevice::WriteOnly))

   // {
//
    //qDebug() << "write json file failed";
//

    //}
    file.write(ba);
    file.close();



}


MainWindow::MainWindow(QWidget *parent, int C, int t, double speed)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , snakeC(C)
    , qtime(new QTimer(this))
    , qq(addToolBar(tr("aaa")))
    , startA(new QAction(QIcon(":/images/start.png"),"start",this))
    , pauseA(new QAction(QIcon(":/images/pause.png"),"pause", this))
    , play1A(new QAction(QIcon(":/images/restart-line.png"),"1 player", this))
    , play2A(new QAction(QIcon(":/images/restart-fill.png"),"2 players", this))
    , saveA(new QAction(QIcon(":/images/save.png"),"save", this))
    , loadA(new QAction(QIcon(":/images/loading.png"),"load", this))
    , exitA(new QAction(QIcon(":/images/exit.png"),"exit", this))
    //, dia(new Dialog(this))
    , times(t)
    , fps(speed)
{
    color = new int[500];
    color[0] = 255;
    for(int i = 1; i < 500; i++){
        color[i] = (double(color[i-1]) * 0.97);
      //、、qDebug() << color[i];
    }

    qq->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    qq->addAction(startA);
    startA->setFont(QFont("Courier New", 16, 70));
    pauseA->setFont(QFont("Courier New", 16, 70));
    play1A->setFont(QFont("Courier New", 16, 70));
    play2A->setFont(QFont("Courier New", 16, 70));
    saveA->setFont(QFont("Courier New", 16, 70));
    loadA->setFont(QFont("Courier New", 16, 70));
    exitA->setFont(QFont("Courier New", 16, 70));
    qq->addAction(pauseA);
    qq->addAction(play1A);
    qq->addAction(play2A);
    qq->addAction(saveA);
    qq->addAction(loadA);
    qq->addAction(exitA);
    connect(startA,SIGNAL(triggered()),this,SLOT(on_pushButton_clicked()));
    connect(pauseA,SIGNAL(triggered()),this,SLOT(on_pushButton_7_clicked()));
    connect(play1A,SIGNAL(triggered()),this,SLOT(on_pushButton_2_clicked()));
    connect(play2A,SIGNAL(triggered()),this,SLOT(on_pushButton_3_clicked()));
    connect(saveA,SIGNAL(triggered()),this,SLOT(on_pushButton_5_clicked()));
    connect(loadA,SIGNAL(triggered()),this,SLOT(on_pushButton_6_clicked()));
    connect(exitA,SIGNAL(triggered()),this,SLOT(on_pushButton_4_clicked()));
    //qq->show();




    st = -1;
    ui->setupUi(this);
    qsrand(QTime::currentTime().msec());
    initGame();
    qtime->setInterval((int)fps);
    connect(qtime, &QTimer::timeout,this,&MainWindow::concon);
    setButton(-1);
    //connect(dia->, SIGNAL(clicked()), this, reset(1));
    //connect(ui->pushButton_2, SIGNAL(click()),this,&MainWindow::reset);
    //qtime->start();
}

MainWindow::~MainWindow()
{
    ss.clear();
    for(int i = 0; i < 100; i++){
        delete [] mms[i];
    }
    delete [] mms;
    delete [] color;
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    MainWindow::reset();
}

void MainWindow::on_pushButton_clicked()
{
    qtime->start();
    st = 1;
    setButton(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    MainWindow::reset(2);
}

void MainWindow::on_pushButton_4_clicked()
{
    close();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this,"标题",".","*.json");
    if (file_name.length() != 0)
    //qDebug() << file_name;
    save(file_name);
}

void MainWindow::on_pushButton_6_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"标题",".","*.json");
    if (file_name.length() != 0)
    load(file_name);
}

void MainWindow::on_pushButton_7_clicked()
{
    st = 0;
    qtime->stop();
    setButton(0);
    repaint();
}

void MainWindow::on_actionpause_triggered()
{
    st = 0;
    qtime->stop();
    setButton(0);
    repaint();
}

void MainWindow::on_actionsave_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(this,"标题",".","*.json");
    //qDebug() << file_name;
    if (file_name.length() != 0)
    save(file_name);
}

void MainWindow::on_actionload_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this,"标题",".","*.json");
    if (file_name.length() != 0)
    load(file_name);
}

void MainWindow::on_actionquit_triggered()
{
    close();
}

void MainWindow::on_actionrestart_triggered()
{
    MainWindow::reset();
}

void MainWindow::on_actionstart_triggered()
{
    MainWindow::on_pushButton_clicked();
}

void MainWindow::on_actioncontinue_triggered()
{
    qtime->start();
    st = 1;
    setButton(1);
}
