#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QtGlobal>


constexpr quint8 START = 0x00;
constexpr quint8 REG = 0x01;
constexpr quint8 ARR = 0x02;

constexpr quint8 ADD = 0x03;
constexpr quint8 SUB = 0x04;
constexpr quint8 MUL = 0x05;

constexpr quint8 SIN = 0x06;
constexpr quint8 COS = 0x07;
constexpr quint8 TAN = 0x08;
constexpr quint8 CTAN = 0x09;

constexpr quint8 DIV = 0x0A;
constexpr quint8 END = 0x0B;

constexpr quint16 DATA_INFO =   0xA0;
constexpr quint16 DATA_IN =     0xD0;
constexpr quint16 DATA_OUT =    0xE0;
constexpr quint16 FORMULA =     0xCA;

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
