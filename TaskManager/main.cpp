#include <QCoreApplication>
#include "taskmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("my-program");

    QCoreApplication::setApplicationVersion("1.0");

    TaskManager manager(&a);


    return a.exec();
}
