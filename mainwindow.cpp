// добавить титульник к справке
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "start.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db.setDatabaseName("DRIVER={ODBC Driver 17 for SQL Server};SERVER=LAPTOP-82D8N0HS\\SQLEXPRESS;"
                       "DATABASE=Aircrafts;Trusted_connection=yes");
    if(!db.open()){
        qDebug() << "Код ошибки: " << db.lastError().text();
        this->close();
    }

    QSqlQuery *q = new QSqlQuery;
    q->exec(QString("SELECT [id], [Название], [Тип ВС], [Страна], [Дата начала эксплуатации],"
                            "[Дата окончания эксплуатации] FROM mt1;"));
    fillingTable(q);

    f.setWindowTitle("Фильтр");

    infWindow.setWindowTitle("Справка о ВС");

    connect(this, SIGNAL(filter_create(QSqlDatabase)), &f, SLOT(creating_filter(QSqlDatabase)));

    connect(&f, SIGNAL(table_update(std::string,
                                    std::string, std::string,
                                    std::string, std::string,
                                    int, int, int, int)),
            this, SLOT(filter_exec(std::string,
                             std::string, std::string,
                             std::string, std::string,
                             int, int, int, int)));

    connect(this, SIGNAL(create_info(QPixmap, QString, QString)), &infWindow, SLOT(creating_inf(QPixmap, QString, QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::check_admin(){
    admin = false;
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // если не админ
}

void MainWindow::on_pushButton_clicked()
{
    close();
    Start *s;
    s = new Start;
    s->setWindowTitle("Вход в систему");
    s->show();
}

void MainWindow::fillingTable(QSqlQuery *q){ // Обновление таблицы
    ui->tableWidget->setSortingEnabled(false); // Выключение сортировки для сохранения целостности данных
    ui->tableWidget->blockSignals(true); // Временная деактивация таблицы
    ui->tableWidget->clear(); // Очистка строк таблицы
    ui->tableWidget->setRowCount(0); // Обнуление количества строк в таблице
    ui->tableWidget->setColumnCount(6); // Указываем число колонок
    ui->tableWidget->setShowGrid(true); // Включаем сетку
    // Разрешаем выделение только одного элемента
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<QString::fromStdString("id")
                                                             <<QString::fromStdString("Название")
                                                             <<QString::fromStdString("Тип ВС")
                                                             <<QString::fromStdString("Страна")
                                                             <<QString::fromStdString("Дата начала эксплуатации")
                                                             <<QString::fromStdString("Дата окончания эксплуатации"));
    // Растягиваем последнюю колонку на всё доступное пространство
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // Скрываем колонку под номером 0
    ui->tableWidget->hideColumn(0);
    int i = 0;
    for(i = 0; q->next(); i++){
        // Вставляем строку
        ui->tableWidget->insertRow(i);
        /* Устанавливаем в первую колонку id забирая его из результата SQL-запроса
         * Эта колонка будет скрыта
         * */
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(q->value(0).toString()));

        // Далее забираем все данные из результата запроса и устанавливаем в остальные поля
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(q->value(1).toString()));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem(q->value(2).toString()));
        ui->tableWidget->setItem(i,3, new QTableWidgetItem(q->value(3).toString()));
        ui->tableWidget->setItem(i,4, new QTableWidgetItem(q->value(4).toString()));
        ui->tableWidget->setItem(i,5, new QTableWidgetItem(q->value(5).toString()));
    }
    if (i == 0){
        qDebug()<<"Mistake in MainWindow::fillingTable(mytrigger, mainWindow.h:65" <<q->lastError();
    }
    ui->tableWidget->blockSignals(false);
    ui->tableWidget->setSortingEnabled(true); // Включение сортировки
    ui->pushButton_4->setEnabled(false); // Выключение кнопки доп.информации о ВС, т.к. нет выбранного ВС
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1) // если изменена строка поиска по названию ВС
{
    title = ui->lineEdit->text().toStdString(); // Запись искомого названия ВС
    updating_table(); // Обновление таблицы
}

void MainWindow::on_pushButton_3_clicked()
{
    emit filter_create(db);
    f.show();
}

void MainWindow::filter_exec(std::string title1,
                             std::string atype1, std::string yearst1,
                             std::string yearend1, std::string country1,
                             int frmin1, int frmax1,
                             int pasmin1, int pasmax2){
    title = title1;
    atype = atype1;
    if(yearst1 != ""){
        yearst = yearst1;
    }
    if(yearend1 != ""){
        yearend = yearend1;
    }
    country = country1;
    if(frmin1 != 0){
        vppmin = frmin1;
    }
    if(frmax1 != 0){
        vppmax = frmax1;
    }
    if(pasmin1 != 0){
        pasmin = pasmin1;
    }
    if(pasmax2 != 0){
        pasmax = pasmax2;
    }
    updating_table();
}

void MainWindow::updating_table(){
    QSqlQuery *q = new QSqlQuery();
    q->exec(QString::fromStdString("SELECT [id], [Название], [Тип ВС], [Страна], [Дата начала эксплуатации],"
                                           "[Дата окончания эксплуатации] FROM mt1 "
                                           "WHERE [Название] LIKE(\'" + title + "%\') AND"
                                                     "[Тип ВС] LIKE(\'" + atype + "%\') AND "
                                                     "[Страна] LIKE(\'" + country + "%\')"
                                           " AND (YEAR([Дата начала эксплуатации]) BETWEEN " + yearst + " AND " + yearend + " OR"
                                            " YEAR([Дата окончания эксплуатации]) BETWEEN " + yearst + " AND " + yearend + " OR"
                                           " " + yearst + " BETWEEN YEAR([Дата начала эксплуатации]) AND YEAR([Дата окончания эксплуатации]) OR"
                                                          " " + yearend + " BETWEEN YEAR([Дата начала эксплуатации]) AND YEAR([Дата окончания эксплуатации]) OR"
                                                       " YEAR([Дата окончания эксплуатации]) IS NULL AND"
                                                       " " + yearend + " >= YEAR([Дата начала эксплуатации]) AND " + yearst + ">= YEAR([Дата начала эксплуатации]) OR "
                                                                                                                                    "" + yearst + " IS NULL AND"
                                                      " " + yearend + " IS NULL)"
                                                       " AND ([Минимальная длина ВПП] BETWEEN CONVERT(INT, " + std::to_string(vppmin) + ") "
                                           "AND CONVERT(INT, " + std::to_string(vppmax) + "));"));
    fillingTable(q);
}


void MainWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    int ind = item->row();
    QSqlQuery query = QSqlQuery(db);
    query.exec(QString::fromStdString("SELECT Название FROM mt1 WHERE id = CONVERT(INT, "
                                      + ui->tableWidget->item(ind, 0)->text().toStdString() + ");"));
    QString str;
    query.first();
    str = query.value(0).toString();
    ui->label_2->setText(str);
    ui->pushButton_4->setEnabled(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    int ind = ui->tableWidget->selectionModel()->currentIndex().row();
    QSqlQuery query = QSqlQuery(db);
    query.exec(QString::fromStdString("SELECT Фото FROM mt1 WHERE id = CONVERT(INT, "
                                                      + ui->tableWidget->item(ind, 0)->text().toStdString() + ");"));
    QPixmap photo;
    query.first();
    if(photo.loadFromData(query.value(0).toByteArray(), "JPG"))
    {
        query.exec(QString::fromStdString("SELECT Справка FROM mt1 WHERE id = CONVERT(INT, "
                                          + ui->tableWidget->item(ind, 0)->text().toStdString() + ");"));
        query.first();
        QString t = query.value(0).toString();
        query.exec(QString::fromStdString("SELECT Название FROM mt1 WHERE id = CONVERT(INT, "
                                          + ui->tableWidget->item(ind, 0)->text().toStdString() + ");"));
        query.first();
        QString info_title = query.value(0).toString();
        infWindow.open();
        emit create_info(photo, t, info_title);
    }
}


void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    std::string str;
    str = ui->tableWidget->item(row, column)->text().toStdString();
    QSqlQuery query = QSqlQuery(db);
    std::string columns[] = {"id", "Название", "[Тип ВС]", "Страна", "[Дата начала эксплуатации]", "[Дата окончания эксплуатации]"};

    if(column == 0){
        return;
    }else if(column == 1 || column == 2 || column == 3){
        query.exec(QString::fromStdString("UPDATE mt1 "
                                          "SET " + columns[column] + " = CAST(\'" + str + "\' AS VARCHAR(50)) WHERE id = " + std::to_string(row + 1) + " ;"));
    }else if(column == 4 || column == 5){
        if(str == ""){
            query.exec(QString::fromStdString("UPDATE mt1 "
                                              "SET " + columns[column] + " = NULL WHERE id = " + std::to_string(row + 1) + " ;"));
        }
        else{
            query.exec(QString::fromStdString("UPDATE mt1 "
                                              "SET " + columns[column] + " = CAST(\'" + str + "\' AS DATE) WHERE id = " + std::to_string(row + 1) + " ;"));
        }
    }
}

