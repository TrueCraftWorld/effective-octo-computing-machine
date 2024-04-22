#include <QCoreApplication>
#include <QCommandLineParser>
#include <QHostAddress>
#include <QFile>

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

    QCommandLineOption nodeAddressLine(QStringList() << "n" << "node_IP",
            QString("<IP-address> of some node from cluster"),
             QString("IP-address"));

    QCommandLineOption nodePortLine(QStringList() << "p" << "node_port",
            QString("<port> of some node from cluster"),
             QString("path"));

    parser.addOption(inputFilePath);
    parser.addOption(outputFilePath);
    parser.addOption(nodeAddressLine);
    parser.addOption(nodePortLine);

    parser.process(app);

    QHostAddress nodeIP(parser.value(nodeAddressLine));
    quint16 nodePort(parser.value(nodePortLine));
    QFile inputFile(parser.value(inputFilePath));
    QFile outputFile(parser.value(outputFilePath));


    return a.exec();
}
