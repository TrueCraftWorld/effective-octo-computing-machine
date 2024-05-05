#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QObject>
#include <QTextStream>
#include <QSharedPointer>
#include "constants.h"

enum class SerialMode {
    SEND_CHAR,
    SEND_DOUBLE,
    OTHER
};

struct DataInfo {
    quint64 dataCount;
    char packType = DATA_INFO;
};

class SerialiZer : public QObject
{
    Q_OBJECT
public:
    explicit SerialiZer(QObject *parent = nullptr);
    void processFormula(QTextStream&);
    void processDataInput(QTextStream&);


signals:
    void messageReady(QSharedPointer<QByteArray>);

private:

    void addCommomInfo(QSharedPointer<QByteArray>);

    SerialMode m_workMode;
};

#endif // SERIALIZER_H
