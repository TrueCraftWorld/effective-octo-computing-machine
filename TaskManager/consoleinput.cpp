#include "consoleinput.h"
#include "constants.h"
#include <iostream>
#include <QTextStream>
#include <QBuffer>

ConsoleInput::ConsoleInput(const ConsoleActions& actionType)
{
//    QTextStream input(&m_array);
    setDevice(&m_buf);
    device()->open(QIODevice::ReadWrite);
    setPadChar(PADDING);
    switch (actionType) {
    case ConsoleActions::DataIn:

        std::cout << "Enter numbers to process, divided by enter. Enter \"quit\" to stop entering" << "\r\n";

        while (1){
            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;
                flush();
//                device()->reset();
                break;
            } else if (QString::fromStdString(line).toDouble() != 0.0) {
                std::cout << "data accepted" << std::endl;
                *(static_cast<QTextStream*>(this)) << QString::fromStdString(line).toDouble();
            } else {
                std::cout << "smth wrong" << std::endl;
                break;
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
                flush();
//                device()->reset();
                break;
            } else if (!QString::fromStdString(line).contains(' ')) {
                std::cout << "data accepted" << std::endl;
                *(static_cast<QTextStream*>(this)) << QString::fromStdString(line);
            } else {
                std::cout << "smth wrong" << std::endl;
                break;
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
                *(static_cast<QTextStream*>(this)) << QString::fromStdString(line);
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
                *(static_cast<QTextStream*>(this)) << QString::fromStdString(line);
                break;
            }
        }

        break;
    }
}