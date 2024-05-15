#include "consoleinput.h"
#include "constants.h"
#include <iostream>
//#include <QTextStream>
#include <QBuffer>
#include <QSharedPointer>
#include <QPointer>


ConsoleInput::ConsoleInput(const ConsoleActions& actionType)
//    : m_buf(new QByteArray())
{
    setVersion(QDataStream::Qt_5_15);
    QPointer<QBuffer> dynamicBuff (new QBuffer(&m_ba));
    setDevice(dynamicBuff);
    device()->open(QIODevice::ReadWrite);
    quint64 dataCount = 0;
    QByteArray tmpBA;
    QDataStream tmpStream(&tmpBA, QIODevice::ReadWrite);
    tmpStream.setVersion(QDataStream::Qt_5_15);
    double tmp = 0;
    switch (actionType) {
    case ConsoleActions::DataIn:

        std::cout << "Enter numbers to process, divided by enter. Enter \"quit\" to stop entering" << "\r\n";


        while (1) {

            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;

                *this << dataCount << tmpBA;
                break;
            } else {
                bool isOk = false;
                tmp = QString::fromStdString(line).toDouble(&isOk);
                if (isOk) {
                    ++dataCount;
                    std::cout << "data accepted" << std::endl;
                    tmpStream << tmp;
                } else {
                    std::cout << "smth wrong, data dropped" << std::endl;
    //                break;
                }
            }
        }

        break;
    case ConsoleActions::DataOut:
        break;
    case ConsoleActions::Formula:
        std::cout << "Enter formula in reverse Polish. every input on new line. Enter \"quit\" to stop entering" << "\r\n";

        while (1){
            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;
                break;
            } else if (!QString::fromStdString(line).contains(' ')) {
                std::cout << "data accepted" << std::endl;

                *this << QString::fromStdString(line);
            } else {
                std::cout << "smth wrong, data dropped" << std::endl;
            }
        }
        break;
    case ConsoleActions::TargetPort:
        std::cout << "Enter port of node" << "\r\n";

        while (1){
            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;
                break;
            } else if (!QString::fromStdString(line).contains(' ')){
                std::cout << "data accepted" << std::endl;
                *this << QString::fromStdString(line);
                break;
            }
        }
        break;
    case ConsoleActions::TargetIP:
        std::cout << "Enter IP4 addr of node in form xxx.xxx.xxx.xxx" << "\r\n";

        while (1) {
            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;
                break;
            } else if (!QString::fromStdString(line).contains(' ')) {
                std::cout << "data accepted" << std::endl;
                *this << QString::fromStdString(line);
                break;
            }
        }
        break;
    }
}

void ConsoleInput::dataOutput(QSharedPointer<QByteArray> &inp)
{
    double tmp = 0;
    unsigned char command = 0;
    *this << *inp;
    device()->reset();
    *this >> command;
    while (!atEnd()){
        *this >> tmp;
        std::cout << tmp << std::endl;
    }
}
