
/**
 *   \file     list_node.cpp
 *   \version  0.02
 *   \date     2024.04.23
 */

#include "list_node.h"


/**
 *   \brief   Конструктор
 *   \param  *parent
 *   \return  Нет
 */
ListNode::ListNode()
{

}


/**
 *   \brief   Добавляем информацию об узле в список
 *   \param   node - информацию об узле
 *   \return  Нет
 */
void ListNode::append(Node node)
{
    list_nodes.append(node);
}


/**
 *   \brief   Удаляем информацию об узле из списока
 *   \param   ip - ip узла
 *   \return  Нет
 */
void ListNode::remove_ip(QString ip)
{
    for (auto iter = list_nodes.begin(); iter != list_nodes.end(); iter++)
    {
        if (iter->get_ip() == ip)
        {
            list_nodes.erase(iter);

            break;
        }
    }
}


/**
 *   \brief   Удаляем информацию об узле из списока
 *   \param   port - порт узла
 *   \return  Нет
 */
void ListNode::remove_port(quint16 port)
{
    for (auto iter = list_nodes.begin(); iter != list_nodes.end(); iter++)
    {
        if (iter->get_port() == port)
        {
            list_nodes.erase(iter);

            break;
        }
    }
}


/**
 *   \brief   Удаляем информацию об узле из списока
 *   \param   i - номер удаляемого узла
 *   \return  Нет
 */
void ListNode::remove_all()
{
    list_nodes.clear();
}


/**
 *   \brief   Возвращаем информацию об узле из списока
 *   \param   ip - ip узла
 *   \return  Информация об узле
 */
Node ListNode::value_ip(QString ip)
{
    for (auto  iter = list_nodes.begin(); iter != list_nodes.end(); iter++)
    {
        if (iter->get_ip() == ip)
        {
            Node node(iter->get_ip(), iter->get_port(), iter->get_priority(), iter->get_mips(), iter->get_throughput());


            return node;

            break;
        }
    }
}


/**
 *   \brief   Возвращаем информацию об узле из списока
 *   \param   ip - ip узла
 *   \return  Информация об узле
 */
Node ListNode::value_port(quint16 port)
{
    for (auto  iter = list_nodes.begin(); iter != list_nodes.end(); iter++)
    {
        if (iter->get_port() == port)
        {
            Node node(iter->get_ip(), iter->get_port(), iter->get_priority(), iter->get_mips(), iter->get_throughput());


            return node;

            break;
        }
    }
}


/**
 *   \brief   Ищем по IP узле в списке
 *   \param   ip - IP адрес узла
 *   \return  Наличие узла в списке, true - есть в списке, false - нет
 */
bool ListNode::find_ip(QString ip)
{
    bool is_node = false;


    for (auto iter = list_nodes.begin(); iter != list_nodes.end(); iter++)
    {
        if (iter->get_port() == ip)
        {
            is_node = true;
        }
    }

    return is_node;
}


/**
 *   \brief   Ищем по порту узле в списке
 *   \param   port - порт узла
 *   \return  Наличие узла в списке, true - есть в списке, false - нет
 */
bool ListNode::find_port(quint16 port)
{
    bool is_node = false;


    for (auto iter = list_nodes.begin(); iter != list_nodes.end(); iter++)
    {
        if (iter->get_port() == port)
        {
            is_node = true;
        }
    }

    return is_node;
}


/**
 *   \brief   Список узлов с которыми данный узел является инициатором установления связи
 *   \param   Нет
 *   \return  Список узлов
 */
QVector<Node> ListNode::list_connected()
{
    QVector<Node> list_node_connected;


    for (auto iter = list_nodes.begin(); iter != list_nodes.end(); iter++)
    {
        if (iter->get_connected() == true)
        {
            list_node_connected.append(*iter);
        }
    }

    return list_node_connected;
}


/**
 *   \brief   Возвращаем информацию о всех узлах в списке
 *   \param   Нет
 *   \return  Информация об узле
 */
void ListNode::list_all()
{
    for (auto iter = list_nodes.begin(); iter != list_nodes.end(); iter++)
    {
        qDebug() << iter->get_ip();
        qDebug() << iter->get_port();
        qDebug() << iter->get_priority();
        qDebug() << iter->get_mips();
        qDebug() << iter->get_throughput();
        qDebug() << iter->get_connected();
    }
}


/**
 *   \brief   Получаем количество элементов в ссписке
 *   \param   Нет
 *   \return  Количество элементов в списке
 */
quint32 ListNode::size()
{
    return list_nodes.size();
}
