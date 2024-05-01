#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include "serializer.h"
#include "fileinput.h"
#include "consoleinput.h"
#include "datacontrol.h"
#include "formulacontrol.h"
#include "tcp_side.h"
#include "udp_side.h"

class TaskManager : public QObject
{

public:
    explicit TaskManager(QObject *parent = nullptr);
    /**
     * @brief initialize
     * @param consoleIP - IP of node to connect
     * @param consolePort - port of nde to connect
     * @param formulaFile - path to formula file
     * @param inputFile - path to data file
     * @param outputFile - path to result file
     * @details sets up connections that shoulf exist all lifetime of TM
     *          all params can be empty Qstring - this invokes console dialog from user
     */
    void initialize(QString consoleIP,
                    QString consolePort,
                    QString formulaFile,
                    QString inputFile,
                    QString outputFile);

    // as in UML but still not sure is it good
    FileInput m_fileReader;
    SerialiZer m_serialiser;
    ConsoleInput m_userConsole;
    FormulaControl m_formula;
    DataControl m_data;
    TCP_Side m_tcp;
    UDP_Side m_udp;

signals:

};

#endif // TASKMANAGER_H
