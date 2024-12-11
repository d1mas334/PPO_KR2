#include "start.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Start *s;
    s = new Start;
    s->setWindowTitle("Вход в систему");
    s->show();
    return a.exec();
}
