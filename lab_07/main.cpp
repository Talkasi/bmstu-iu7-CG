#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Лабораторная №7 по компьютерной графике");
    w.setMaximumSize(QSize(2000, 966));
    w.show();
    return a.exec();
}
