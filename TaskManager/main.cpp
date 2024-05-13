#include <QCoreApplication>
#include "taskmanager.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("my-program");

    QCoreApplication::setApplicationVersion("1.0");

//    QByteArray arr;
//    QDataStream str(&arr, QIODevice::ReadWrite);
//    double val = 6;

//    unsigned char d = 0x83;
//    str << d;
//    str << val;

//    val = 0.1;
//    str << val;

//    val = 0.6;
//    str << val;

//    val = 123;
//    str << val;

//    val = 9000;
//    str << val;

//    val = 505.1;
//    str << val;

//    str.device()->reset();
//    unsigned char d2 = 0;
//    str >> d2;
//    std::cout << d2 << std::endl;
////    while (!str.atEnd()) {
////        double val2;
////        str >> val2;
////        std::cout << val2 << std::endl;
////    }
//    ConsoleInput ble (ConsoleActions::DataOut);
//    d = 0x83;
//    ble << d;

//    val = 6;
//    ble << val;

//    val = 0.1;
//    ble << val;

//    val = 0.6;
//    ble << val;

//    val = 123;
//    ble << val;

//    val = 9000;
//    ble << val;

//    val = 505.1;
//    ble << val;
//    ble.device()->reset();

//    AbstractOutput* blu = static_cast<AbstractOutput*>(&ble);

//    *blu >> d2;
//    std::cout << d2 << std::endl;
//    while (!blu->atEnd()) {
//        double val2;
//        *blu >> val2;
//        std::cout << val2 << std::endl;
//    }

    TaskManager manager(&a);


    return a.exec();
}
