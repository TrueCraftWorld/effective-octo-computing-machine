#include "dataoutputreader.h"

#include <QDataStream>
#include <QTextStream>

DataOutputReader::DataOutputReader(QObject *parent)
    : QObject{parent}
{

}

void DataOutputReader::setFile(QFile * n_file)
{
    m_file = n_file;
}

void DataOutputReader::saveToFile()
{
    if(!m_file->open(QIODevice::ReadWrite | QIODevice::Text)) return;
    QTextStream tStream(m_file);
    QDataStream dstream(&m_data,QIODevice::ReadWrite);
    double value = 0;
    QString str;
    for (int var = 0; var < m_data.size(); var+=8) {
        dstream >> value;
        str = QString("%1\r\n").arg(value);
        tStream << str;
    }
    tStream.flush();
    m_file->close();
}

void DataOutputReader::setData(QByteArray & n_arr)
{
    m_data = n_arr;
    saveToFile();
}
