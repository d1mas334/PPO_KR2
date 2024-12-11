#ifndef FILTER_H
#define FILTER_H

#include <QDialog>

namespace Ui {
class Filter;
}

class Filter : public QDialog
{
    Q_OBJECT

public:
    explicit Filter(QWidget *parent = nullptr);
    ~Filter();

private:
    Ui::Filter *ui;
};

#endif // FILTER_H
