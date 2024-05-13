#include <QCoreApplication>
#include <QDebug>
#include "mocknode.h"
#include <iostream>
#include "nodeserializer.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    // udp network instance
    // add QT += network to your project file
    //NodeUdpManager net(nullptr, 1010);
    MockNode node(45578);



    return a.exec();
}