#include <QCoreApplication>
#include "core.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Core core;
    core.startServer();
    return a.exec();
}
