#include "UI/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TerrainGenerator* tg = new TerrainGenerator();
    MainWindow w(tg);
    w.show();
    return a.exec();
}
