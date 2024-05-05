#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QtGlobal>


constexpr char START = 0x00;
constexpr char REG = 0x01;
constexpr char ARR = 0x02;

constexpr char ADD = 0x03;
constexpr char SUB = 0x04;
constexpr char MUL = 0x05;

constexpr char SIN = 0x06;
constexpr char COS = 0x07;
constexpr char TAN = 0x08;
constexpr char CTAN = 0x09;

constexpr char DIV = 0x0A;
constexpr char END = 0x0B;

constexpr char DATA_INFO = 0xA0;
constexpr char DATA_IN = 0xD0;
constexpr char DATA_OUT = 0xE0;
constexpr char FORMULA = 0xCA;

constexpr quint16 KEY = 132;

constexpr char PADDING = '\n';

enum class ConsoleActions : int {
    Formula,
    DataIn,
    DataOut,
    TargetIP,
    TargetPort
};

#endif // CONSTANTS_H
