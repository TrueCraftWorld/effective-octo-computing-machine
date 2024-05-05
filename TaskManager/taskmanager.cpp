#include "taskmanager.h"
#include "constants.h"

TaskManager::TaskManager(QObject *parent)
    : QObject{parent}
{

}

void TaskManager::initialize(StartParams &param)
{
    if (param.targetIP.isEmpty()) {

    }
    if (param.targetPort.isEmpty()) {

    }


    //idea is that we use text stram in bothcases - console input OR file reading
    //so we just initilizing those streams differently;
    if (param.formulaFilePath.isEmpty()) {
         m_formulaStream = static_cast<QTextStream*>(new ConsoleInput(ConsoleActions::Formula));
    } else {
        m_formulaStream = static_cast<QTextStream*>(new FileInput(QString("/home/kikorik/Data/formula.txt")));
//        m_formulaStream = static_cast<QTextStream*>(new FileInput(param.formulaFilePath));
    }

    if (param.inputFilePath.isEmpty()) {
        m_inputStream = static_cast<QTextStream*>(new ConsoleInput(ConsoleActions::DataIn));
    } else {
        m_inputStream = static_cast<QTextStream*>(new FileInput(QString("/home/kikorik/Data/input.txt")));
//        m_inputStream = static_cast<QTextStream*>(new FileInput(param.inputFilePath));
    }


    if (param.outputFilePath.isEmpty()) {
//        m_outputStream = new FileInput(QString("results.txt")); //
    } else {
        m_outputStream = static_cast<QTextStream*>(new FileInput(param.outputFilePath));
    }

    //maybe this should be in serializer
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
