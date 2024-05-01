#include <QCoreApplication>
#include <QCommandLineParser>
#include <QHostAddress>
#include <QFile>

#include "datainputparser.h"
#include "dataoutputreader.h"
#include "formulainputparser.h"
#include "networklayer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("my-copy-program");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;

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

    parser.addOption(inputFilePath);
    parser.addOption(outputFilePath);
    parser.addOption(formulaFilePath);
    parser.addOption(nodeAddressLine);
    parser.addOption(nodePortLine);

    parser.process(a);

    //new content
    //gathering QStrings from app console params



//    QHostAddress nodeIP(parser.value(nodeAddressLine));
//    quint16 nodePort(parser.value(nodePortLine).toInt());
//    QFile inputFile(parser.value(inputFilePath));
//    QFile formulaFile(parser.value(formulaFilePath));
//    QFile outputFile(parser.value(outputFilePath));

//    DataInputParser inputData;
//    NetworkLayer netConnect;
//    FormulaInputParser formula;
//    DataOutputReader fileSaver;
//    fileSaver.setFile(&outputFile);

//    QObject::connect(&inputData, &DataInputParser::dataReady, &netConnect, &NetworkLayer::sendData);
//    QObject::connect(&formula, &FormulaInputParser::formulaReady, &netConnect, &NetworkLayer::sendData);
//    QObject::connect(&netConnect, &NetworkLayer::dataReady, &fileSaver, &DataOutputReader::setData);
//    netConnect.socketSetup();
//    netConnect.connectTo(nodeIP, nodePort);

//    formula.processFormulaFile(formulaFile);

//    inputData.processFile(inputFile);





    return a.exec();
}
