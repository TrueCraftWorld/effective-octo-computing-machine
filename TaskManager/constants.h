#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QtGlobal>


constexpr unsigned char START = 0x00;
constexpr unsigned char REG = 0x01;
constexpr unsigned char ARR = 0x02;

constexpr unsigned char ADD = 0x03;
constexpr unsigned char SUB = 0x04;
constexpr unsigned char MUL = 0x05;

constexpr unsigned char SIN = 0x06;
constexpr unsigned char COS = 0x07;
constexpr unsigned char TAN = 0x08;
constexpr unsigned char CTAN = 0x09;

constexpr unsigned char DIV = 0x0A;
constexpr unsigned char END = 0x0B;

constexpr unsigned char DATA_INFO =   0xA0;
constexpr unsigned char DATA_IN =     0xD0;
constexpr unsigned char DATA_OUT =    0xE0;
constexpr unsigned char FORMULA =     0xCA;

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
