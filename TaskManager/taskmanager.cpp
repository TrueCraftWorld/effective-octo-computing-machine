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
    m_formula.processFormulaFile(*m_formulaStream);
    if (param.inputFilePath.isEmpty()) {
        m_inputStream = static_cast<QTextStream*>(new ConsoleInput(ConsoleActions::DataIn));
    } else {
        m_inputStream = static_cast<QTextStream*>(new FileInput(QString("/home/kikorik/Data/input.txt")));
//        m_inputStream = static_cast<QTextStream*>(new FileInput(param.inputFilePath));
    }
    m_data.processFile(*m_inputStream);

    if (param.outputFilePath.isEmpty()) {
//        m_outputStream = new FileInput(QString("results.txt")); //
    } else {
        m_outputStream = static_cast<QTextStream*>(new FileInput(param.outputFilePath));
    }


}

void TaskManager::processData(QDataStream *)
{
    //serialize

    //send
}
