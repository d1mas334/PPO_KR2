#include "start.h"
#include "ui_start.h"

Start::Start(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);
    ui->pushButton_3->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->label_3->setVisible(false);
    ui->lineEdit->setEchoMode(QLineEdit::Password);
    w.setWindowTitle("Воздушные суда гражданской авиации");

    connect(this, SIGNAL(is_notadmin()), &w, SLOT(check_admin()));
}

Start::~Start()
{
    delete ui;
}

void Start::on_pushButton_2_clicked()
{
    this->close();
    w.show();
    emit is_notadmin();
}

void Start::on_pushButton_clicked()
{
    ui->pushButton_3->setVisible(true);
    ui->lineEdit->setVisible(true);
}

void Start::on_pushButton_3_clicked()
{
    std::string str = ui->lineEdit->text().toStdString();
    if(str != "12345"){
        ui->label_3->setVisible(true);
        return;
    }
    this->close();
    w.show();
}
