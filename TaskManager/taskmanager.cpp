#include "taskmanager.h"
#include "constants.h"

TaskManager::TaskManager(QObject *parent)
    : QObject{parent}
{

}

void TaskManager::initialize(StartParams &param)
{

    if (param.targetIP.isEmpty()) {
        QString str;
        ConsoleInput consDial(ConsoleActions::TargetIP);
        consDial >> str;
        m_tergetNode.ip4Addr = QHostAddress(str);
    } else {
        m_tergetNode.ip4Addr = QHostAddress(param.targetIP);
    }
    if (param.targetPort.isEmpty()) {
        QString str;
        ConsoleInput consDial_2(ConsoleActions::TargetPort);
        consDial_2 >> str;
        m_tergetNode.port = str.toInt();
    } else {
        m_tergetNode.port = (param.targetPort).toInt();
    }
    m_tergetNode.soc = new QTcpSocket();
    m_tergetNode.soc->connectToHost(m_tergetNode.ip4Addr, m_tergetNode.port);

    QObject::connect(&m_serialiser, &SerialiZer::messageReady, this, [this](const QSharedPointer<QByteArray> msg){
        this->m_tcp_side.SendMessageToNode(m_tergetNode.soc, *msg);
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
    if (param.outputFilePath.isEmpty()) {
//        m_outputStream = new FileInput(QString("results.txt")); //
    } else {
        m_outputStream = static_cast<QTextStream*>(new FileInput(param.outputFilePath));
    }

    m_serialiser.processFormula(*m_formulaStream);
    m_serialiser.processDataInput(*m_inputStream);


}

void TaskManager::processData(QTextStream *)
{
    //serialize
//    if (1) m_serialiser.getCharWithDelimeter();
//    else m_serialiser.getInDataStreamFormat();
    //send
}
