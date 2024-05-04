#include "datacontrol.h"
#include "constants.h"

#include <QTextStream>
#include <QDataStream>

DataControl::DataControl()
{

}

void DataControl::processFile(QTextStream & input)
{
    m_data.clear();
    input.device()->reset();

    QDataStream stream(&m_data, QIODevice::ReadWrite);
    while (!input.atEnd()) { // выглядит как оверкилл, но количество строк считаем и кастим к байт арррею
        QString line;
        input >> line;
        stream << line;
        m_linesCount++;
    }
    input.device()->close();
    emit dataReady(m_data, DATA_IN);
//    someFile.close();
}
