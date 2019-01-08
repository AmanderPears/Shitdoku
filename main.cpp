#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.visual();
    w.layout()->setSizeConstraint(QLayout::SetFixedSize);
    w.show();

    return a.exec();
}
