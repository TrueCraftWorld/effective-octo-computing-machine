#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "serializer.h"
#include "fileinput.h"
#include "consoleinput.h"
#include "tcpserver.h"
#include <QCommandLineParser>
#include <QSharedPointer>

struct StartParams {
    QString targetIP;
    QString targetPort;
    QString formulaFilePath;
    QString inputFilePath;
    QString outputFilePath;
};

struct TargetNode {
    QHostAddress ip4Addr;
    quint16 port;
    QTcpSocket* soc;
};

class TaskManager : public QObject
{

public:
    explicit TaskManager(QObject *parent = nullptr);
    explicit TaskManager(QCoreApplication *parent = nullptr);

    void initialize(StartParams& param);


signals:
    void dataRecieved();

private:
    SerialiZer m_serialiser;
    Server m_tcp_side;
    QCommandLineParser m_startParamParser;
    TargetNode m_targetNode;

    QTextStream *m_formulaStream;
    QTextStream *m_inputStream;
    AbstractOutput *m_outputStream;

};

#endif // TASKMANAGER_H
