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

    if (param.targetIP.isEmpty()) {
        QString str;
        ConsoleInput consDial(ConsoleActions::TargetIP);
        consDial >> str;
        m_targetNode.ip4Addr = QHostAddress(str);
    } else {
        m_targetNode.ip4Addr = QHostAddress(param.targetIP);
    }
    if (param.targetPort.isEmpty()) {
        QString str;
        ConsoleInput consDial_2(ConsoleActions::TargetPort);
        consDial_2 >> str;
        m_targetNode.port = str.toInt();
    } else {
        m_targetNode.port = (param.targetPort).toInt();
    }

    m_targetNode.soc = new QTcpSocket();
    m_targetNode.soc->connectToHost(m_targetNode.ip4Addr, m_targetNode.port);

    QObject::connect(&m_serialiser, &SerialiZer::messageReady, this,
                     [this](QSharedPointer<QByteArray> msg) {
        m_tcp_side.SendMessageToNode(m_targetNode.soc, msg);
    });
    QObject::connect(&m_tcp_side, &Server::signalSendDataToSerializer, &m_serialiser,  &SerialiZer::processReturnData);

    QObject::connect(&m_serialiser, &SerialiZer::resultsAccepted, this,
                     [this](QSharedPointer<QByteArray> msg) {
        m_inputStream = new QTextStream(msg.get());
        QString tmp;
        while (!m_inputStream->atEnd()) {
            *m_inputStream >> tmp;
            std::cout << tmp.toStdString() << std::endl;
        }
    });

    //idea is that we use text stram in bothcases - console input OR file reading
    //so we just initilizing those streams differently;
    if (param.formulaFilePath.isEmpty()) { 
         m_formulaStream = static_cast<QTextStream*>(new ConsoleInput(ConsoleActions::Formula));
    } else {
        m_formulaStream = static_cast<QTextStream*>(new FileInput(param.formulaFilePath));
    }

    if (param.inputFilePath.isEmpty()) {
        m_inputStream = static_cast<QTextStream*>(new ConsoleInput(ConsoleActions::DataIn));
    } else {
        m_inputStream = static_cast<QTextStream*>(new FileInput(param.inputFilePath));
    }

    //TODO

//    if (param.outputFilePath.isEmpty()) {
//        m_outputStream = new FileInput(QString("results.txt")); //
//    } else {
//        m_outputStream = static_cast<QTextStream*>(new FileInput(param.outputFilePath));
//    }

    m_serialiser.processFormula(*m_formulaStream);
    m_serialiser.processDataInput(*m_inputStream);
}

