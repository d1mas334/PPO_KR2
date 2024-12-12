#include "information.h"
#include "ui_information.h"

Information::Information(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Information)
{
    ui->setupUi(this);
}

Information::~Information()
{
    delete ui;
}

void Information::creating_inf(QPixmap photo, QString t){
    ui->label->setPixmap(photo.scaled(360, 190));
    ui->textBrowser->setText(t);
}

void Information::on_pushButton_clicked()
{
    this->close();
}
