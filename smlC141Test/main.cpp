#include "mainwindow.h"
#include<QScreen>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QScreen *screen=QGuiApplication::primaryScreen();
    QRect mm=screen->availableGeometry();
    int screenWidth=mm.width();
    int screenHeight=mm.height();
    MainWindow w;
    w.resize(screenWidth,screenHeight);
    w.show();
    return a.exec();
}
