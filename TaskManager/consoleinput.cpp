#include "consoleinput.h"
#include "constants.h"
#include <iostream>
#include <QTextStream>
#include <QBuffer>
#include <QSharedPointer>


ConsoleInput::ConsoleInput(const ConsoleActions& actionType)
{
    setDevice(&m_buf);
    device()->open(QIODevice::ReadWrite);
    switch (actionType) {
    case ConsoleActions::DataIn:

        std::cout << "Enter numbers to process, divided by enter. Enter \"quit\" to stop entering" << "\r\n";

        while (1){
            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;
                flush();
                break;
            } else {
                bool isOk = false;
                double tmp = QString::fromStdString(line).toDouble(&isOk);
                if (isOk) {
                    std::cout << "data accepted" << std::endl;
                    *this << QString::fromStdString(line).toDouble() << PADDING;
                } else {
                    std::cout << "smth wrong, data dropped" << std::endl;
    //                break;
                }
            }
        }
        break;
    case ConsoleActions::DataOut:
        //TODO
        //justfine
        break;
    case ConsoleActions::Formula:
        std::cout << "Enter formula in reverse Polish. every input on new line. Enter \"quit\" to stop entering" << "\r\n";

        while (1){
            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;
                flush();
                break;
            } else if (!QString::fromStdString(line).contains(' ')) {
                std::cout << "data accepted" << std::endl;
                *this << QString::fromStdString(line) << PADDING;
            } else {
                std::cout << "smth wrong, data dropped" << std::endl;
//                break;
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
                flush();
                break;
            } else if (!QString::fromStdString(line).contains(' ')){
                std::cout << "data accepted" << std::endl;
                *this << QString::fromStdString(line) << PADDING;
                break;
            }
        }
        break;
    case ConsoleActions::TargetIP:
        std::cout << "Enter IP4 addr of node in form xxx.xxx.xxx.xxx" << "\r\n";

        while (1){
            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;
                flush();
                break;
            } else if (!QString::fromStdString(line).contains(' ')) {
                std::cout << "data accepted" << std::endl;
                *this << QString::fromStdString(line) << PADDING;
                break;
            }
        }

        break;
    }
}

void ConsoleInput::dataOutput(QSharedPointer<QByteArray> inp)
{
    QString tmp;
    *this << *inp;
    flush();
    device()->reset();

    while (!atEnd()){
        *this >> tmp;
        std::cout << tmp.toStdString() << std::endl;
    }
}
