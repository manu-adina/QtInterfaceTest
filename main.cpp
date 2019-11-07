#include "rosgroundcontrol.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ROSGroundControl w;
    w.show();

    return a.exec();
}
