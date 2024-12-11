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

    connect(this, SIGNAL(filter_create(QSqlDatabase)), &f, SLOT(creating_filter(QSqlDatabase)));

    connect(&f, SIGNAL(table_update(std::string,
                                    std::string, std::string,
                                    std::string, std::string,
                                    int, int, int, int)),
            this, SLOT(filter_exec(std::string,
                             std::string, std::string,
                             std::string, std::string,
                             int, int, int, int)));
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

void MainWindow::fillingTable(QSqlQuery *q){
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(6); // Указываем число колонок
    ui->tableWidget->setShowGrid(true); // Включаем сетку
    // Разрешаем выделение только одного элемента
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем заголовки колонок
    //QStringList headers;

    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<QString::fromStdString("id")
                                                             <<QString::fromStdString("Название")
                                                             <<QString::fromStdString("Тип ВС")
                                                             <<QString::fromStdString("Страна")
                                                             <<QString::fromStdString("Дата начала эксплуатации")
                                                             <<QString::fromStdString("Дата окончания эксплуатации"));
    // Растягиваем последнюю колонку на всё доступное пространство
    //ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // Скрываем колонку под номером 0
    ui->tableWidget->hideColumn(0);
    int i = 0;
    for(/*int*/ i = 0; q->next(); i++){
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
        qDebug()<<"Mistake in MainWindow::fillingTable(mytrigger, mainWindow.h:65";
    }
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


