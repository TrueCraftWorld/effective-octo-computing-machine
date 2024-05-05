#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "serializer.h"
#include "fileinput.h"
#include "consoleinput.h"
#include "tcpserver.h"

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
    /**
     * @brief initialize
     */
    void initialize(StartParams& param);
    void processData(QTextStream *);



//    TCP_Side m_tcp;
//    UDP_Side m_udp;

signals:
    void dataReady(QTextStream *);

private:
    SerialiZer m_serialiser;
    Server m_tcp_side;

    QTextStream *m_formulaStream;
    QTextStream *m_inputStream;
    QTextStream *m_outputStream;
    TargetNode m_tergetNode;

};

#endif // TASKMANAGER_H
