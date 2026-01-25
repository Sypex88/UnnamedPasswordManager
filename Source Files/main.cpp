#include "mainwindow.h"

#include <QApplication>

#include "loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginwindow l;
    l.show();
    return a.exec();
}
