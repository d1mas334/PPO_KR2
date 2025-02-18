#include "start.h"

#include <QApplication>

int main(int argc, char *argv[])   // основной модуль программы
{
    QApplication a(argc, argv);  // переменная приложения
    Start *s;  // указатель на переменную типа стартовое окно
    s = new Start;  // создание объекта типа стартовое окно
    s->setWindowTitle("Вход в систему");  // настройка заголовка стартового окна
    s->show();  // отображение стартового окна
    return a.exec();  // запуск приложения
}
