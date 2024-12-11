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

void Information::creating_inf(QPixmap photo, std::string t){
    ui->label->setPixmap(photo.scaled(360, 190));
}

void Information::on_pushButton_clicked()
{
    this->close();
}
