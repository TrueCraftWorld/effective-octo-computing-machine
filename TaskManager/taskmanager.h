#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include "serializer.h"
#include "fileinput.h"
#include "consoleinput.h"
#include "tcp_side.h"
#include "udp_side.h"

struct StartParams {
    QString targetIP;
    QString targetPort;
    QString formulaFilePath;
    QString inputFilePath;
    QString outputFilePath;
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

    SerialiZer m_serialiser;
//    FormulaControl m_formula;
//    DataControl m_data;
    TCP_Side m_tcp;
    UDP_Side m_udp;

signals:
    void dataReady(QTextStream *);

private:
    QTextStream *m_formulaStream;
    QTextStream *m_inputStream;
    QTextStream *m_outputStream;

};

#endif // TASKMANAGER_H
