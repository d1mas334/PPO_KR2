#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filter.h"
#include "QSqlDatabase"
// #include <QSqlRelationalTableModel>
#include "QSqlError"
#include "QSqlQuery"
// #include <QSqlQueryModel>
// #include "QSqlTableModel"
#include "information.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
        void fillingTable(QSqlQuery *q);

private:
    Ui::MainWindow *ui;
    Filter f;
    Information infWindow;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    bool admin = true;
    std::string title = "", atype = "", yearst = "1900", yearend = "2024", country = "";
    int vppmin = 0, vppmax = 9999999, pasmin = 0, pasmax = 9999;
};
#endif // MAINWINDOW_H
