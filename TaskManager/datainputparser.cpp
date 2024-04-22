#include "datainputparser.h"

#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QByteArray>

DataInputParser::DataInputParser(QObject *parent)
    : QObject{parent}
{

}

void DataInputParser::processFile(QFile & someFile)
{
    if(!someFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    m_data.clear();
    QTextStream inputStream(&someFile);
    QDataStream stream;
    while (!inputStream.atEnd()) { // выглядит как оверкилл, но количество строк считаем и кастим к байт арррею
        QString line = inputStream.readLine();
        stream << line.toDouble();
    }
    stream >> m_data;
    emit dataReady(m_data);
}
