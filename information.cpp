#include "information.h"
#include "ui_information.h"

Information::Information(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Information)
{
    ui->setupUi(this);
}

Information::~Information()  // при закрытии окна с информацией об отдельном ВС
{
    delete ui;
}

  // создание окна с информацией об отдельном ВС
void Information::creating_inf(QPixmap photo, QString t, QString info_title){
    ui->label->setPixmap(photo.scaled(360, 190));
    ui->textBrowser->setText(t);
    ui->label_2->setText(info_title);
}

void Information::on_pushButton_clicked()  // при нажатии на кнопку "Назад"
{
    this->close();
}
