#include "mainwindow.h"
#include <iostream>

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << a.instance()->thread()->currentThreadId();
    MainWindow w;
    w.show();
    a.exec();

    return 0;
}
