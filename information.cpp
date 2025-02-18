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

void Information::creating_inf(QPixmap photo, QString t, QString info_title){
    ui->label->setPixmap(photo.scaled(360, 190));
    ui->textBrowser->setText(t);
    ui->label_2->setText(info_title);
}

void Information::on_pushButton_clicked()
{
    this->close();
}
