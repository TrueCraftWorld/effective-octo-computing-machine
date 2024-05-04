#include "formulacontrol.h"
#include "constants.h"

#include <QTextStream>
#include <QDataStream>

FormulaControl::FormulaControl(QObject *parent)
    : QObject{parent}
{

}

void FormulaControl::processFormulaFile(QTextStream & input)
{
    m_action.clear();
    input.device()->reset();
//    QByteArray arr;
    QDataStream stream(&m_action,QIODevice::ReadWrite);
    stream << START;
    while (!input.atEnd()) { // выглядит как оверкилл, но количество строк считаем и кастим к байт арррею
        QString line;
        input >> line;
        if (line.contains(QString("+"))) {
            stream << ADD;
        } else if (line.contains(QString("-"))) {
            stream << SUB;
        } else if (line.contains(QString("*"))) {
            stream << MUL;
        } else if (line.contains(QString("/"))) {
            stream << DIV;
        } else if (line.contains(QString("S"))) {
            stream << SIN;
        } else if (line.contains(QString("C"))) {
            stream << COS;
        } else if (line.contains(QString("T"))) {
            stream << TAN;
        } else if (line.contains(QString("Y"))) {
            stream << CTAN;
        } else if (line.contains(QString("X"))) {
            stream << ARR;
        } else {
            // Перед тем, как добавлять число в строку, нужно добавить команду REG
            stream << REG;
            stream << (line);
        }
    }
    stream << END;
//    stream << Qt::endl;
    input.device()->close();
//    stream >> m_action;
    emit formulaReady(m_action, FORMULA);
}
