
/**
 *   \file     node.cpp
 *   \version  0.01
 *   \date     2024.04.23
 */

#include "node.h"


/**
 *   \brief   Конструктор по умолчанию
 *   \param   Нет
 *   \return  Нет
 */
Node::Node()
{
    ip = QHostAddress();
    port = 0;
    priority = 0;
    mips = 0.0;
    throughput = 0.0;
}


/**
 *   \brief   Конструктор с параметрами
 *   \param   ip - IP адрес узла в сети
 *   \param   port - порт узла в сети
 *   \param   priority - приоритет узла в сети
 *   \param   mips - вычислительная мощность узла в сети
 *   \param   throughput - пропускная способность до узла
 *   \return  Нет
 */
Node::Node(QString ip, quint16 port, quint32 priority, double mips, double throughput) : ip(QHostAddress(ip)),
    port(port),
    priority(priority),
    mips(mips),
    throughput(throughput),
    connected(false)
{
}


/**
 *   \brief   Сохраняем IP адрес
 *   \param   ip - IP адрес
 *   \return  Нет
 */
void Node::set_ip(QString ip)
{
    this->ip = QHostAddress(ip);
}


/**
 *   \brief   Сохраняем номер порта
 *   \param   port - номер порта
 *   \return  Нет
 */
void Node::set_port(quint16 port)
{
    this->port = port;
}


/**
 *   \brief   Сохраняем приоритет
 *   \param   priority - приоритет
 *   \return  Нет
 */
void Node::set_priority(quint32 priority)
{
    this->priority = priority;
}


/**
 *   \brief   Сохраняем вычислительную мощность
 *   \param   mips - вычислительная мощность
 *   \return  Нет
 */
void Node::set_mips(double mips)
{
    this->mips = mips;
}


/**
 *   \brief   Сохраняем пропускную способность
 *   \param   throughput - пропускная способность
 *   \return  Нет
 */
void Node::set_throughput(double throughput)
{
    this->throughput = throughput;
}


/**
 *   \brief   Сохраняем признак установления связи с узлом
 *   \param   connected - признак связи с узлом, true - инициатор связи, false - приемник связи
 *   \return  Нет
 */
void Node::set_connected(bool connected)
{
    this->connected = connected;
}


/**
 *   \brief   Возвращаем IP адрес
 *   \param   Нет
 *   \return  IP адрес
 */
QString Node::get_ip()
{
    return ip.toString();
}


/**
 *   \brief   Возвращаем номер порта
 *   \param   Нет
 *   \return  Номер порта
 */
quint16 Node::get_port()
{
    return port;
}


/**
 *   \brief   Возвращаем приоритет
 *   \param   Нет
 *   \return  Приоритет
 */
quint32 Node::get_priority()
{
    return priority;
}


/**
 *   \brief   Возвращаем вычислительную мощьность
 *   \param   Нет
 *   \return  Вычислительная мощьность
 */
double Node::get_mips()
{
    return mips;
}


/**
 *   \brief   Возвращаем пропускную способность
 *   \param   Нет
 *   \return  Пропускная способность
 */
double Node::get_throughput()
{
    return throughput;
}


/**
 *   \brief   Возвращаем признак установления связи с узлом
 *   \param   Нет
 *   \return  true - инициатор связи, false - приемник связи
 */
bool Node::get_connected()
{
    return connected;
}
