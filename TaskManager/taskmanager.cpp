#include "taskmanager.h"
#include "constants.h"
#include <iostream>

TaskManager::TaskManager(QObject *parent)
    : QObject{parent}
{


}

TaskManager::TaskManager(QCoreApplication *parent)
    : QObject{parent}
{
    QCommandLineOption inputFilePath(QStringList() << "i" << "input",
            QString("<path> to file with data to compute"),
             QString("path"));

    QCommandLineOption outputFilePath(QStringList() << "o" << "output",
            QString("<path> to file to save computed data"),
             QString("path"));

    QCommandLineOption formulaFilePath(QStringList() << "f" << "formula",
            QString("<path> to file to read formula from"),
             QString("path"));

    QCommandLineOption nodeAddressLine(QStringList() << "n" << "node_IP",
            QString("<IP-address> of some node from cluster"),
             QString("IP-address"));

    QCommandLineOption nodePortLine(QStringList() << "p" << "node_port",
            QString("<port> of some node from cluster"),
             QString("path"));

    m_startParamParser.addOption(inputFilePath);
    m_startParamParser.addOption(outputFilePath);
    m_startParamParser.addOption(formulaFilePath);
    m_startParamParser.addOption(nodeAddressLine);
    m_startParamParser.addOption(nodePortLine);

    m_startParamParser.process(*parent);

    StartParams startParams;
    startParams.formulaFilePath = m_startParamParser.value(formulaFilePath);
    startParams.inputFilePath = m_startParamParser.value(inputFilePath);
    startParams.outputFilePath = m_startParamParser.value(outputFilePath);
    startParams.targetIP = m_startParamParser.value(nodeAddressLine);
    startParams.targetPort = m_startParamParser.value(nodePortLine);
    initialize(startParams);
}

void TaskManager::initialize(StartParams &param)
{

    QObject::connect(&m_serialiser, &SerialiZer::messageReady, this,
                     [this](QSharedPointer<QByteArray> &msg) {
        m_tcp_side.SendMessageToNode(m_targetNode.soc, *msg);
    });

    QObject::connect(&m_tcp_side, &TcpModule::signalSocketConnected, this,
                     [this](QTcpSocket* socketForConnect, QHostAddress ip4, quint16 port)
    {
        this->m_targetNode.soc = socketForConnect;
    });

    QObject::connect(&m_tcp_side, &TcpModule::signalSendDataToSerializer, &m_serialiser,  &SerialiZer::processReturnData);

    QObject::connect(&m_serialiser, &SerialiZer::resultsAccepted, this,
                     [this](QSharedPointer<QByteArray> &msg) {
        m_outputStream->dataOutput(msg);
    });

    if (param.targetIP.isEmpty()) {
        QString str;
        ConsoleInput consDial(ConsoleActions::TargetIP);
        consDial >> str;
        m_targetNode.node.ip = QHostAddress(str);
    } else {
        m_targetNode.node.ip = QHostAddress(param.targetIP);
    }

    if (param.targetPort.isEmpty()) {
        QString str;
        ConsoleInput consDial_2(ConsoleActions::TargetPort);
        consDial_2 >> str;
        m_targetNode.node.port = str.toInt();
    } else {
        m_targetNode.node.port = (param.targetPort).toInt();
    }

    m_tcp_side.slotConnectSocket(m_targetNode.node);

    //idea is that we use text stram in bothcases - console input OR file reading
    //so we just initilizing those streams differently;
    if (param.formulaFilePath.isEmpty()) { 
         m_formulaStream = (new ConsoleInput(ConsoleActions::Formula));
    } else {
        m_formulaStream = (new FileInput(param.formulaFilePath));
    }

    if (param.inputFilePath.isEmpty()) {
        m_inputStream = (new ConsoleInput(ConsoleActions::DataIn));
    } else {
        m_inputStream = (new FileInput(param.inputFilePath));
    }

    if (param.outputFilePath.isEmpty()) {
        m_outputStream = static_cast<AbstractOutput*>(new ConsoleInput(ConsoleActions::DataOut)); //
    } else {
        m_outputStream = static_cast<AbstractOutput*>(new FileInput(param.outputFilePath)); //
    }

    m_serialiser.processInput(m_inputStream, m_formulaStream);
//    m_serialiser.processFormula(m_formulaStream);
}

