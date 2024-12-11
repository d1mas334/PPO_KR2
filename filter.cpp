#include "filter.h"
#include "ui_filter.h"

Filter::Filter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filter)
{
    ui->setupUi(this);
}

Filter::~Filter()
{
    delete ui;
}
