#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include "ui_dialog.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    int k;
    friend class MainWindow;
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Dialog *ui;

};

#endif // DIALOG_H
