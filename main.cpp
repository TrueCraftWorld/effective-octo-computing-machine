
/**
 *   \file     main.cpp
 *   \version  0.02
 *   \date     2024.04.23
 */

#include <QCoreApplication>
#include "cluster_node.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ClusterNode cls;
    return a.exec();
}
