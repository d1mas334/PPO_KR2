#ifndef FILTER_H
#define FILTER_H

#include <QDialog>
#include "QDialogButtonBox"
#include "QSqlQuery"

namespace Ui {
class Filter;
}

class Filter : public QDialog
{
    Q_OBJECT

public:
    explicit Filter(QWidget *parent = nullptr);
    ~Filter();
signals:
    void table_update(std::string title1,
                      std::string type1, std::string yearst1,
                      std::string yearend1, std::string country1,
                      int frmin1, int frmax1,
                      int pasmin1, int pasmax1);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void creating_filter(QSqlDatabase q);

private:
    Ui::Filter *ui;
};

#endif // FILTER_H
