
/**
 *   \file     main.cpp
 *   \version  0.02
 *   \date     2024.05.07
 *   \mainpage
 */

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "node.h"


static void parser_cmd_line(const QStringList &arg, Options_command_line &options_command_line);


/**
 *   \brief   Точка входа в программу
 *   \param   argc - количество аргументов командной строки
 *   \param  *argv[] - строка аргументов командной строки
 *   \retval  Код завершения программы
 */
int main(int argc, char *argv[])
{
    QCoreApplication a_node(argc, argv);
    QCoreApplication::setApplicationName("Node");
    QCoreApplication::setApplicationVersion("0.3");
    Options_command_line options_command_line = { static_cast<quint16>(Udp_socket_setting::MULTICAST_PORT),
                                                  default_ip,
                                                  static_cast<quint16>(Udp_socket_setting::LOCAL_MODE_TRANSMIT_PORT),
                                                  static_cast<quint16>(Udp_socket_setting::LOCAL_MODE_RECEIVE_PORT),
                                                  //QHostAddress(QHostAddress::LocalHost).toString(),
                                                  QHostAddress(QHostAddress::AnyIPv4).toString(),
                                                  false,
                                                  false };

    qDebug() << "Узел";
    qDebug() << QCoreApplication::applicationVersion();
    parser_cmd_line(QCoreApplication::arguments(), options_command_line);
    Node *node = new Node(&a_node, options_command_line);
    node->setObjectName("node");

    return a_node.exec();
}


/**
 *   \brief   Функция анализирует переданные аргументы
 *   \param  &arg - строка с аргументами командной строки
 *   \param  &options_command_line - структура содержащая состояния опций
 *   \retval  Нет
 */
static void parser_cmd_line(const QStringList &arg, Options_command_line &options_command_line)
{
    QCommandLineOption cmd_line_option_verbose(QStringList() << "verbose", QCoreApplication::translate("main", "Подробный вывод."));
    QCommandLineOption cmd_line_option_local_mode(QStringList() << "lm", QCoreApplication::translate("main", "Перевод узла в локальный режим работы (49002 - порт приема пакетов на стороне узла, а 49003 - порт назначения пакетов, отправляемых узлом)."));
    QCommandLineOption cmd_line_option_multicast_ip(QStringList() << "ip",
                                                    QCoreApplication::translate("main", "Задаем мультикастовый IP адрес. По умолчанию используется 225.1.1.130."),
                                                    QCoreApplication::translate("main", "IP адрес"));
    QCommandLineOption cmd_line_option_port(QStringList() << "port",
                                            QCoreApplication::translate("main", "Задаем порт. Диапазон доступных портов от 1024 до 65535. По умолчанию используется 49001."),
                                            QCoreApplication::translate("main", "номер_порта"));
    QCommandLineParser cmd_line_parser;


    cmd_line_parser.setApplicationDescription("Узел, консольное приложение, образует вычислительную сеть из себе подобных, принимает данные по сети и выполняет вычисления.");
    cmd_line_parser.addHelpOption();
    cmd_line_parser.addVersionOption();
    cmd_line_parser.addOption(cmd_line_option_verbose);
    cmd_line_parser.addOption(cmd_line_option_local_mode);
    cmd_line_parser.addOption(cmd_line_option_multicast_ip);
    cmd_line_parser.addOption(cmd_line_option_port);
    cmd_line_parser.process(arg);

    options_command_line.verbose = cmd_line_parser.isSet(cmd_line_option_verbose);

    options_command_line.local_mode = cmd_line_parser.isSet(cmd_line_option_local_mode);
    if (!options_command_line.local_mode)
    {
        qDebug() << "Узел работает в нормальном режиме.";
        if ("" != cmd_line_parser.value(cmd_line_option_multicast_ip) && default_ip != cmd_line_parser.value(cmd_line_option_multicast_ip))
        {
            QRegExp reg("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");


            if (cmd_line_parser.value(cmd_line_option_multicast_ip).contains(reg))
            {
                options_command_line.multicast_ip = cmd_line_parser.value(cmd_line_option_multicast_ip);
                qDebug() << "Установлен IP " << options_command_line.multicast_ip;
            }
            else
            {
                qDebug() << "Передан не верный IP адрес.";
                qDebug() << "Будет установлен IP адрес по умолчанию," << default_ip;
            }
        }
        else
        {
            qDebug() << "Установлен IP по умолчанию," << default_ip;
        }

        QString port_str = cmd_line_parser.value(cmd_line_option_port);
        bool convert_ok;
        quint16 port = static_cast<quint16>(port_str.toInt(&convert_ok));


        if (convert_ok && (port > 1023) && port != static_cast<quint16>(Udp_socket_setting::MULTICAST_PORT))
        {
            options_command_line.multicast_port = port;
            qDebug() << "Установлен порт " << options_command_line.multicast_port;
        }
        else if (port <= 1023)
        {
            qDebug() << "Задан порт из зарезервированного диапазона.";
            qDebug() << "Установлен порт по умолчанию," << static_cast<quint16> ( Udp_socket_setting::MULTICAST_PORT );
        }
        else
        {
            qDebug() << "Установлен порт по умолчанию," << static_cast<quint16> ( Udp_socket_setting::MULTICAST_PORT );
        }
    }
    else
    {
        qDebug() << "Узел работает в локальном режиме.";
    }
}
