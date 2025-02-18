#include "start.h"
#include "ui_start.h"

Start::Start(QWidget *parent):  // описание стартового окна
    QDialog(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);
    ui->pushButton_3->setVisible(false);
    ui->passwordLine->setVisible(false);
    ui->wrongPassword->setVisible(false);
    ui->passwordLine->setEchoMode(QLineEdit::Password);
    w.setWindowTitle("Воздушные суда гражданской авиации");

    connect(this, SIGNAL(is_notadmin()), &w, SLOT(check_admin()));
}

Start::~Start()  // если стартовое окно закрыли
{
    delete ui;
}

void Start::on_pushButton_2_clicked()  // вход как гость если нажата кнопка "Гость"
{
    this->close();
    w.show();
    emit is_notadmin();
}

// предостваление возможности ввести пароль администратора если нажата кнопка "Администратор"
void Start::on_pushButton_clicked()
{
    ui->pushButton_3->setVisible(true);
    ui->passwordLine->setVisible(true);
}

// проверка пароля, если нажата кнопка "Войти" и вход как администратор в случае, если пароль верен
void Start::on_pushButton_3_clicked()
{
    std::string str = ui->passwordLine->text().toStdString();
    if(str != "12345"){
        ui->wrongPassword->setVisible(true);
        return;
    }
    this->close();
    w.show();
}
