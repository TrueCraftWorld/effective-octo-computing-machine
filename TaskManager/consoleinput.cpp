#include "consoleinput.h"
#include "constants.h"
#include <iostream>
#include <QTextStream>
#include <QBuffer>

ConsoleInput::ConsoleInput(const ConsoleActions& actionType)
{
//    QTextStream input(&m_array);
    this->setDevice(&m_buf);
    this->device()->open(QIODevice::ReadWrite);
    switch (actionType) {
    case ConsoleActions::DataIn:

        std::cout << "Enter numbers to process, divided by enter. Enter \"quit\" to stop entering" << "\r\n";

        while (1){
            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;
                flush();
                device()->reset();
                break;
            } else if (QString::fromStdString(line).toDouble() != 0.0) {
                std::cout << "data accepted" << std::endl;
                *(static_cast<QTextStream*>(this)) << QString::fromStdString(line).toDouble() << PADDING;
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
                *(static_cast<QTextStream*>(this)) << QString::fromStdString(line) << PADDING;
            } else {
                std::cout << "smth wrong" << std::endl;
                break;
            }
        }
        break;
    case ConsoleActions::TargetPort:
        break;
    case ConsoleActions::TargetIP:
        break;
    }
}
