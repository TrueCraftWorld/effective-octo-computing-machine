#include "datainputparser.h"
#include "networklayer.h"

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
    QDataStream stream(&m_data, QIODevice::ReadWrite);
    while (!inputStream.atEnd()) { // выглядит как оверкилл, но количество строк считаем и кастим к байт арррею
        QString line = inputStream.readLine();
        stream << line.toDouble();
    }
//    stream.flush();
    emit dataReady(m_data, DATA_IN);
}
