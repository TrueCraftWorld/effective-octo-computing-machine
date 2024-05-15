#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QObject>
#include <QDataStream>
#include <QSharedPointer>
#include "constants.h"
#include "abstractoutput.h"

enum class SerialMode {
    SEND_CHAR,
    SEND_DOUBLE,
    OTHER
};

struct DataInfo {
    quint64 dataCount;
    char packType = PKG_DATAINFO;
};

class SerialiZer : public QObject
{
    Q_OBJECT
public:
    explicit SerialiZer(QObject *parent = nullptr);
    void processInput(AbstractOutput* data, AbstractOutput* formula);

    void processReturnData(QSharedPointer<QByteArray>&);

signals:
    void messageReady(QSharedPointer<QByteArray>&);
    void resultsAccepted(QSharedPointer<QByteArray>&);

private:
    void processFormula(AbstractOutput*);
//    void processDataInput(AbstractOutput*);
//    void addCommomInfo(QSharedPointer<QByteArray>);

    SerialMode m_workMode = SerialMode::SEND_CHAR;
};

#endif // SERIALIZER_H
