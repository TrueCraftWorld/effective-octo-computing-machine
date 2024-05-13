
/**
 *   \file     data_storage_processing.cpp
 *   \version  0.04
 *   \date     2024.05.12
 */

#include "data_storage_processing.h"

#include <cmath>
#include <QRegularExpression>


static bool compare_pair(const QPair<QString, QVector<double>> &value1,
                 const QPair<QString, QVector<double>> &value2);
static bool compare_double(const double value1, const double value2);

/**
 *   \brief   Конструктор
 *   \param  *parent - указатель на родительский класс
 *   \param   node_info - данные узлов
 *   \param   np_data - количество обрабатываемых данных
 *   \retval  Нет
 */
DataStorageProcessing::DataStorageProcessing(QObject *parent,
                                             bool is_selected_node,
                                             NodeInfo &node_info,
                                             quint64 amount_processed_data)
    : QObject{parent},
    is_selected_node(is_selected_node),
    amount_processed_data(amount_processed_data),
    amount_data_process(0)
{
    if (!is_selected_node)
    {
        init_worker();
    }
    else
    {
        init_tasker(node_info);
    }
}


/**
 *   \brief   Деструктор
 *   \param   Нет
 *   \retval  Нет
 */
DataStorageProcessing::~DataStorageProcessing()
{
}


/**
 *   \brief   Инициализация как планировщака задач
 *   \param   node_info - данные узлов
 *   \retval  Нет
 */
void DataStorageProcessing::init_tasker(NodeInfo &node_info)
{
    data_tasker.clear();

    for (auto it = node_info.neighbour_nodes.begin(); it != node_info.neighbour_nodes.end(); ++it)
    {
        QString str = it->node_id.ip.toString() + ":" + QString::number(it->node_id.port);


        data_tasker.append(QPair<QString, QVector<double>> (str, QVector<double>(0.0)));
        amount_data_nodes.push_back(it->mips);
    }

    QString str = ":" + QString::number(node_info.port_host);
    data_tasker.append(QPair<QString, QVector<double>> (str, QVector<double>(0.0)));
    amount_data_nodes.push_back(node_info.mips);

    auto [max, min] = std::minmax_element(amount_data_nodes.begin(), amount_data_nodes.end());
    double dmin = *min;
    double dmax = *max;

    for (quint32 i = 0; i < amount_data_nodes.size(); ++i)
    {
        amount_data_nodes[i] = 0.1 + (amount_data_nodes[i] - dmin) / (dmax - dmin) * (1.1 - 0.1);
    }

    double sum = std::accumulate(amount_data_nodes.begin(), amount_data_nodes.end(), 0.0);

    for (quint32 i = 0; i < amount_data_nodes.size(); ++i)
    {
        amount_data_nodes[i] = 100 / sum * amount_data_nodes[i] / 100;
    }

    sum = 0.0;
    quint32 i = 0;

    for (auto it = data_tasker.begin(); it != data_tasker.end(); ++it)
    {
        amount_data_nodes[i] = std::ceil(amount_processed_data * amount_data_nodes[i]);
        sum += amount_data_nodes[i];

        if (sum > amount_processed_data)
        {
            amount_data_nodes[i] -= (sum - amount_processed_data);
        }

        it->second.resize(std::ceil(amount_data_nodes[i]));
        i++;
    }

    std::sort(data_tasker.begin(), data_tasker.end(), compare_pair);
    std::sort(amount_data_nodes.begin(), amount_data_nodes.end(), compare_double);

    for (auto it = data_tasker.begin(); it != data_tasker.end(); ++it)
    {
        qDebug() << it->first << it->second.size();
        it->second.clear();
    }

    /* TODO: для отработки, удалить, начало */
    // QVector<double> data;

    // amount_data_process = 990000;
    // data.resize(10000);
    // for (quint32 i = 0; i < data.size(); ++i)
    // {
        // data[i] = i * 1.0;
    // }
    // data_tasker[0].second.resize(amount_data_process);
    // fill_data(data);
    /* TODO: для отработки, удалить, конец */
}


/**
 *   \brief   Инициализация как вычислительного узла
 *   \param   Нет
 *   \retval  Нет
 */
void DataStorageProcessing::init_worker()
{
    data_worker.clear();
}


/**
 *   \brief   Заполнение данных
 *   \param   data - данные для обработки
 *   \retval  Нет
 */
void DataStorageProcessing::fill_data(QVector<double> &data)
{
    if (amount_processed_data >= (amount_data_process + data.size()))
    {
        if (!is_selected_node)
        {
            data_worker.append(data);
            amount_data_process += data_worker.size() - amount_data_process;
        }
        else
        {
            quint32 amount_required_nodes = search_required_nodes(data.size());

            if (amount_data_process == 0)
            {
                quint64 size_qvectors = 0;

                for (quint32 i = 0; i < amount_required_nodes; ++i)
                {
                    size_qvectors += amount_data_nodes[i];

                    if (data.size() < size_qvectors)
                    {
                        data_tasker[i].second = data.mid(amount_data_process, data.size() - amount_data_process);
                        amount_data_process += data.size();
                    }
                    else
                    {
                        data_tasker[i].second = data.mid(amount_data_process, amount_data_nodes[i]);
                        amount_data_process += amount_data_nodes[i];
                    }
                }
            }
            else
            {
                quint32 free_node = search_free_nodes();
                quint64 size_qvectors = 0;
                QVector<double> buf = data_tasker[free_node].second.mid(0, amount_data_process);
                quint64 buf_size = buf.size();

                for (quint32 i = free_node; i < amount_required_nodes; ++i)
                {
                    size_qvectors += amount_data_nodes[i];

                    if (data.size() < size_qvectors - amount_data_process)
                    {
                        data_tasker[i].second.clear();
                        data_tasker[i].second = buf;
                        data_tasker[i].second += data.mid(0, data.size());
                        amount_data_process += data.size();
                    }
                    else if (!buf.isEmpty())
                    {
                        data_tasker[i].second.clear();
                        data_tasker[i].second = buf;
                        data_tasker[i].second += data.mid(0, amount_data_nodes[i] - amount_data_process);
                        amount_data_process += amount_data_nodes[i] - amount_data_process;
                        buf.clear();
                    }
                    else
                    {
                        data_tasker[i].second = data.mid(amount_data_process - buf_size, amount_data_nodes[i]);
                        amount_data_process += amount_data_nodes[i];
                    }
                }
            }
        }
    }
}


/**
 *   \brief   Функция возвращает данные планировщика задач
 *   \param   Нет
 *   \retval  Данные планировщика задач
 */
QVector<QPair<QString, QVector<double>>> &DataStorageProcessing::get_data_tasker()
{
    return data_tasker;
}


/**
 *   \brief   Функция возвращает данные вычислительного узла
 *   \param   Нет
 *   \retval  Данные вычислительного узла
 */
QVector<double> &DataStorageProcessing::get_data_worker()
{
    return data_worker;
}


/**
 *   \brief   Поиск свободного узла
 *   \param   Нет
 *   \retval  Свободный узел
 */
quint32 DataStorageProcessing::search_free_nodes()
{
    quint32 free_node = 0;
    quint64 size_nodes = 0;


    for (quint32 i = 0; i < data_tasker.size(); ++i)
    {
        if (size_nodes < amount_data_process)
        {
            size_nodes += amount_data_nodes[i];
            free_node = i;
        }
        else
        {
            free_node = i - 1;

            break;
        }
    }

    return free_node;
}


/**
 *   \brief   Поиск необходимых узлов
 *   \param   Нет
 *   \retval  Количество необходимых узлов
 */
quint32 DataStorageProcessing::search_required_nodes(quint32 size)
{
    quint32 amont_required_nodes = 1;
    quint64 size_nodes = amount_data_nodes[0];


    for (quint32 i = 1; i < data_tasker.size(); ++i)
    {
        if (size_nodes < size + amount_data_process)
        {
            size_nodes += amount_data_nodes[i];
            amont_required_nodes = i + 1;
        }
        else
        {
            amont_required_nodes = i;

            break;
        }
    }

    return amont_required_nodes;
}


/**
 *   \brief   Предикат для сортировки
 *   \param   value1 - первое значение
 *   \param   value2 - второе значение
 *   \retval  true если первое значение больше второго и false в противном случае
 */
static bool compare_pair(const QPair<QString, QVector<double>> &value1,
                 const QPair<QString, QVector<double>> &value2)
{
    return (value1.second.size() > value2.second.size());
}


/**
 *   \brief   Предикат для сортировки
 *   \param   value1 - первое значение
 *   \param   value2 - второе значение
 *   \retval  true если первое значение больше второго и false в противном случае
 */
static bool compare_double(const double value1, const double value2)
{
    return (value1 > value2);
}