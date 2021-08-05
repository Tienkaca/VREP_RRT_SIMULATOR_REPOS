#include "RRT_VREP_SIMULATOR.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RRT_VREP_SIMULATOR w;
    w.show();
    return a.exec();
}
