#include "serializer.h"

#include "constants.h"
#include <iostream>
#include <QDataStream>
#include <QSharedPointer>
#include <QDebug>


SerialiZer::SerialiZer(QObject *parent)
    : QObject{parent}
{

}

void SerialiZer::processDataInput(AbstractOutput * input)
{
    QSharedPointer<QByteArray> dataStorage (new QByteArray());

    input->device()->reset(); //sets position to 0 in all IO diveces but Qstring
    quint64 dataCount = 0;

    QDataStream stream(dataStorage.get(), QIODevice::ReadWrite);
    stream.setVersion(QDataStream::Qt_5_15);
    *input >> dataCount;

    QSharedPointer<QByteArray> dataInfo (new QByteArray());  //dataInfo litrally can be formed only in here
    dataInfo->clear();
    QDataStream dataInfoStream(dataInfo.get(), QIODevice::ReadWrite);
    dataInfoStream.setVersion(QDataStream::Qt_5_15);
    dataInfoStream << DATA_INFO << dataCount;


    emit messageReady(dataInfo);
    qDebug() << "dataInfo";
    stream << DATA_IN;

    while (!input->atEnd()) {

        double line = 0;
        *input >> line;
        stream << line;
        std::cout << line << std::endl;
    }

//    stream.flush();
    input->device()->close();
    emit messageReady(dataStorage);
    qDebug() << "sentData";
}

void SerialiZer::processReturnData(QSharedPointer<QByteArray> arr)
{

    QDataStream input(arr.get(), QIODevice::ReadWrite);
    input.setVersion(QDataStream::Qt_5_15);

   unsigned char packageId;
   input >> packageId;

   //NEED to decomment line below in prod
   if (packageId != DATA_OUT) return;

   emit resultsAccepted(arr);
}

void SerialiZer::processFormula(AbstractOutput * input)
{
    QSharedPointer<QByteArray> dataStorage (new QByteArray());
    input->device()->reset();
    if (m_workMode == SerialMode::SEND_CHAR) {
//        QTextStream stream(dataStorage.get(),QIODevice::ReadWrite);
        QDataStream stream(dataStorage.get(),QIODevice::ReadWrite);
        stream.setVersion(QDataStream::Qt_5_15);
        stream << START;
        while (!input->atEnd()) { // выглядит как оверкилл, но количество строк считаем и кастим к байт арррею
            QString line;
            *input >> line;
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
            } else if (!line.isEmpty()){
                stream << REG;
                stream << (line.toDouble());
            }

        }
        stream << END;
    }
    emit messageReady(dataStorage);
    qDebug() << "sentFormula";
}
