#ifndef FORMULAINPUTPARSER_H
#define FORMULAINPUTPARSER_H

#include <QObject>
#include <QFile>

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

class FormulaInputParser : public QObject
{
    Q_OBJECT
public:
    explicit FormulaInputParser(QObject *parent = nullptr);
    void processFormulaFile(QFile&);
signals:
    void formulaReady(QByteArray& );
private:
    QByteArray m_action;
};

#endif // FORMULAINPUTPARSER_H
