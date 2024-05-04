#include "consoleinput.h"
#include "constants.h"
#include <iostream>
#include <QDataStream>

ConsoleInput::ConsoleInput(const ConsoleActions& actionType)
{
    switch (actionType) {
    case ConsoleActions::DataIn:
        std::cout << "Enter numbers to process, divided by enter. Enter \"quit\" to stop entering";

        while (1){
            std::string line;
            std::getline(std::cin, line);
            if (line == "quit") {
                std::cout << "quit!" << std::endl;
            } else if (QString::fromStdString(line).toDouble() != 0.0) {

            } else {
                std::cout << "Echo: " << line << std::endl;
                std::cout << "> " << std::flush;
            }
        }
        break;
    case ConsoleActions::DataOut:
        break;
    case ConsoleActions::Formula:
        break;
    case ConsoleActions::TargetPort:
        break;
    case ConsoleActions::TargetIP:
        break;
    }
}
