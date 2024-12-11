#ifndef START_H
#define START_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Start;
}

class Start : public QDialog
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = nullptr);
    ~Start();

signals:
    void is_notadmin();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Start *ui;
    MainWindow w;
};

#endif // START_H
