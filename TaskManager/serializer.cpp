#include "serializer.h"
#include "constants.h"
#include <QTextStream>
#include <QSharedPointer>
#include <QDebug>


SerialiZer::SerialiZer(QObject *parent)
    : QObject{parent}
{

}

void SerialiZer::processDataInput(QTextStream &input)
{
//    QT_NO_CAST_FROM_BYTEARRAY;
    QSharedPointer<QByteArray> dataStorage (new QByteArray());

    input.device()->reset(); //sets position to 0 in all IO diveces but Qstring
    quint64 dataCount = 0;
    QTextStream stream(dataStorage.get(), QIODevice::ReadWrite);
    QString line;
    input >> line;
    dataCount = line.toInt();

    QSharedPointer<QByteArray> dataInfo (new QByteArray());  //dataInfo litrally can be formed only in here
    dataInfo->clear();
    //sending datainfo ahead
    QTextStream dataInfoStream(dataInfo.get(), QIODevice::ReadWrite);
//    QByteArray dataInfoStupidAddOn = QByteArrayLiteral("\xA0");
    dataInfoStream << PADDING << dataCount << PADDING;
    dataInfoStream.flush();
    dataInfo->prepend(DATA_INFO);
    emit messageReady(dataInfo);
    qDebug() << "sentInfo";

    while (!input.atEnd()) {
        line.clear();
        input >> line;
        stream << line << PADDING;
//        dataCount++;
    }
//    stream.seek(0);
    stream.flush();
    input.device()->close();



    //awakward way to add info to bytearray
    QByteArray packInfo = QByteArrayLiteral("\xD0");
    packInfo.append(PADDING);
    dataStorage->prepend(packInfo);
    emit messageReady(dataStorage);
    qDebug() << "sentData";
}

void SerialiZer::processReturnData(QSharedPointer<QByteArray> arr)
{
   QTextStream input(arr.get());
   char packageId;
   input >> packageId;

   //NEED to decomment line below in prod
//   if (packageId != DATA_OUT) return;

   emit resultsAccepted(arr);
}

void SerialiZer::processFormula(QTextStream & input)
{
    QSharedPointer<QByteArray> dataStorage (new QByteArray());
    input.device()->reset();
    if (m_workMode == SerialMode::SEND_CHAR) {
        QTextStream stream(dataStorage.get(),QIODevice::ReadWrite);
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
                stream << REG << PADDING;
                stream << (line);
            }
            stream  << PADDING;
        }
        stream << END;
        stream.flush();
        input.device()->close();
    }
    QByteArray packInfo;
    QTextStream stream2(packInfo, QIODevice::ReadWrite);
    stream2 << FORMULA << PADDING;
    stream2.flush();
    dataStorage->prepend(packInfo);
    emit messageReady(dataStorage);
    qDebug() << "sentFormula";
}
