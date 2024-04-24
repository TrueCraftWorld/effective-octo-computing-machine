#include "formulainputparser.h"
#include "networklayer.h"
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QDataStream>

FormulaInputParser::FormulaInputParser(QObject *parent)
    : QObject{parent}
{

}

void FormulaInputParser::processFormulaFile(QFile & someFile)
{
    if(!someFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream inputStream(&someFile);
    m_action.clear();
    QDataStream stream;
    stream << START;
    while (!inputStream.atEnd()) { // выглядит как оверкилл, но количество строк считаем и кастим к байт арррею
        QString line = inputStream.readLine();
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
            stream << REG << (line.toDouble());
        }

        //прочитали
        //если X оставили как есть
    }
    stream << END;
    stream >> m_action;
    emit formulaReady(m_action, FORMULA);
}
