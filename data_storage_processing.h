
/**
 *   \file     data_storage_processing.h
 *   \version  0.03
 *   \date     2024.05.11
 */

#ifndef DATASTORAGEPROCESSING_H
#define DATASTORAGEPROCESSING_H

#include <QObject>
#include <QMap>
#include <QVector>

#include "node_info.h"


class DataStorageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit DataStorageProcessing(QObject *parent,
                                   bool is_selected_node,
                                   NodeInfo &node_info,
                                   quint64 amount_processed_data = 0);
    ~DataStorageProcessing();
    void fill_data(QVector<double> &data);

signals:

public slots:


private:
    QVector<QPair<QString, QVector<double>>> data_tasker;  /// данные для обработки в режиме планировщика задач
    QVector<double> data_worker;  /// данные для обработки в режиме вычислительного узла
    QVector<double> amount_data_nodes;  /// нормированная производительность, количество данных на узелы
    quint64 amount_data_process;  /// количество данных на обработку
    quint64 amount_processed_data;  /// количество обрабатываемых данных
    //quint64 data_in_vector;  /// количество данных в векторе
    //quint32 count_node;  /// количество узлов в сети
    bool is_selected_node;  ///

    void init_tasker(NodeInfo &node_info);  /// инициализация в режиме планировщика задач
    void init_worker();  /// инициализация в режиме вычислительного узла
    quint32 search_required_nodes(quint32 size);  /// поиск необходимых узлов
    quint32 search_free_nodes(quint64 amount_data_process);  /// поиск свободного узла
};

#endif // DATASTORAGEPROCESSING_H
