#include "taskmanager.h"

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

    } else {
        QTextStream stream = FileInput(param.formulaFilePath).getStream();
    }
    if (param.inputFilePath.isEmpty()) {

    } else {
        QTextStream stream = FileInput(param.formulaFilePath).getStream();
    }
    if (param.outputFilePath.isEmpty()) {

    } else {
        QTextStream stream = FileInput(param.formulaFilePath).getStream();
    }


}
