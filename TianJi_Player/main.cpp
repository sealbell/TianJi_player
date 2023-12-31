#include "mainwindow.h"

#include <QApplication>

//- 程序入口，初始化和启动应用程序。

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
