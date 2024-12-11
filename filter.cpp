#include "filter.h"
#include "ui_filter.h"

Filter::Filter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filter)
{
    ui->setupUi(this);
    ui->label_2->setText("<font color=red>Начальный год должен быть меньше или равен конечному году</font>");
    ui->label_2->hide();
    //ui->buttonBox->
}

Filter::~Filter()
{
    delete ui;
}

void Filter::creating_filter(QSqlDatabase q){
    QStringList str;
    str<<"";
    QSqlQuery query = QSqlQuery(q);
    query.exec("SELECT DISTINCT [Тип ВС] FROM mt1 ORDER BY [Тип ВС];");
    while(query.next()){
        str<<query.value(0).toString();
    }
    ui->comboBox1->clear();
    ui->comboBox1->insertItems(0, str);

    QStringList str2;
    str2<<"";
    query.exec("SELECT DISTINCT [Страна] FROM mt1 ORDER BY [Страна];");
    while(query.next()){
        str2<<query.value(0).toString();
    }
    ui->comboBox2->clear();
    ui->comboBox2->insertItems(0, str2);
}


void Filter::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button->text() == "Закрыть" || button->text() == "Close"){
        this->close();
    }

    if (button->text() == "Сбросить" || button->text() == "Reset"){
        ui->titleEdit->setText("");
        ui->comboBox1->setCurrentText(""); ui->yearEdit_7->setValue(1900);
        ui->yearEdit_8->setValue(2024); ui->comboBox2->setCurrentText("");
        ui->yearEdit_9->setValue(0); ui->yearEdit_10->setValue(999999);
            ui->yearEdit_11->setValue(0); ui->yearEdit_12->setValue(9999);
        emit table_update("",
                          "", "1900",
                          "2024", "",
                          0, 0,
                          0, 0);
    }

    if (button->text() == "Отмена" || button->text() == "Cancel"){
        this->close();
    }

    if (button->text() == "Применить" || button->text() == "Apply"){
        if(ui->yearEdit_7->text().toInt() > ui->yearEdit_8->text().toInt()){
            ui->label_2->show();
            return;
        }
        if(ui->comboBox1->currentText().toStdString() == ""){
            qDebug()<<"filtercpp:69 mistake";
        }
        emit table_update(ui->titleEdit->text().toStdString(),
                          ui->comboBox1->currentText().toStdString(), ui->yearEdit_7->text().toStdString(),
                          ui->yearEdit_8->text().toStdString(), ui->comboBox2->currentText().toStdString(),
                          ui->yearEdit_9->text().toInt(), ui->yearEdit_10->text().toInt(),
                          ui->yearEdit_11->text().toInt(), ui->yearEdit_12->text().toInt());
    }
}
