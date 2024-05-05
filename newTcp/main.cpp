#include <QCoreApplication>
#include <QDebug>
#include "mocknode.h"
#include <iostream>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    // udp network instance
    // add QT += network to your project file
    //NodeUdpManager net(nullptr, 1010);
    quint16 q;
    std::cin >> q;
    MockNode node(q);



    return a.exec();
}