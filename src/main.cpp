#include "View/MainWin.h"
#include <QApplication>

#include <QImage>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CompilerMainWin w;
    w.show();
    return a.exec();

}
