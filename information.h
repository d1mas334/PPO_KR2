#ifndef INFORMATION_H
#define INFORMATION_H

#include <QDialog>

namespace Ui {
class Information;
}

class Information : public QDialog
{
    Q_OBJECT

public:
    explicit Information(QWidget *parent = nullptr);
    ~Information();

private slots:
    void creating_inf(QPixmap photo, std::string t);

    void on_pushButton_clicked();

private:
    Ui::Information *ui;
};

#endif // INFORMATION_H
