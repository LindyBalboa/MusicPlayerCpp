#include "mainwindow.h"
#include <iostream>

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QSplitter::handle#libraryTreeTableSplitter {border: 1px solid rgb(240,240,250);\
                                                                              width:  0px;\
                                                                              }\
                     QTableView::item::selected::active {color: black;\
                                                         background: qlineargradient(x1:1, y1:1, x2:1, y2:0, stop:0 #E0F0FF, stop:0.85 #E0F0FF, stop:1 white);\
                                                         border-top: 1px solid #A3D1FF;\
                                                         border-bottom: 1px solid #A3D1FF;\
                                                         }\
                     QTableView::item                   {color: black;}\
                  ");
    qDebug() << a.instance()->thread()->currentThreadId();
    MainWindow w;
    //w.show();
    a.exec();

    return 0;
}
