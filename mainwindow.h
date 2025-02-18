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
#include "filter.h"
#include "qtablewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void filter_create(QSqlDatabase q);
    void create_info(QPixmap photo, QString t, QString info_title);


private slots:
    void fillingTable(QSqlQuery *q);

    void filter_exec(std::string title1,
                     std::string type1, std::string yearst1,
                     std::string yearend1, std::string country1,
                     int frmin1, int frmax1,
                     int pasmin1, int pasmax2);

    void updating_table();

    void check_admin();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_tableWidget_cellChanged(int row, int column);

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
