
/**
 *   \file     data_storage_processing.h
 *   \version  0.04
 *   \date     2024.05.12
 */

#ifndef DATASTORAGEPROCESSING_H
#define DATASTORAGEPROCESSING_H

#include "node_info.h"

#include <QObject>
#include <QMap>
#include <QVector>


class DataStorageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit DataStorageProcessing(QObject *parent,
                                   bool is_selected_node,
                                   NodeInfo &node_info,
                                   quint64 amount_processed_data = 0);
    ~DataStorageProcessing();

signals:
    void data_tasker_ready(QVector<QPair<QString, QVector<double>>> &data_tasker);
    void data_worker_ready(QVector<double> &data_worker);
    void modified_data_tasker_ready(QVector<QPair<QString, QVector<double>>> &data_tasker);
    void modified_data_worker_ready(QVector<double> &data_worker);

public slots:
    void fill_data(QTcpSocket* socket, QSharedPointer<QVector<double>> ptr_data);
    void fill_modified_data(QTcpSocket* socket, QSharedPointer<QVector<double>> ptr_data);
    void set_formula(QTcpSocket* socket, QSharedPointer<QByteArray> ptr_formula);
    QByteArray &get_formula();

private:
    QVector<QPair<QString, QVector<double>>> data_tasker;  /// данные для обработки в режиме планировщика задач
    QVector<double> data_worker;  /// данные для обработки в режиме вычислительного узла
    QVector<double> amount_data_nodes;  /// нормированная производительность, количество данных на узелы
    quint64 amount_data_process;  /// количество данных на обработку
    quint64 amount_processed_data;  /// количество обрабатываемых данных
    QByteArray formula;  /// формула
    bool is_selected_node;  /// признак в каком режиме работает узел (планировщик задач или вычислительный узел)

    void init_tasker(NodeInfo &node_info);  /// инициализация в режиме планировщика задач
    void init_worker();  /// инициализация в режиме вычислительного узла
    quint32 search_required_nodes(quint32 size);  /// поиск необходимых узлов
    quint32 search_free_nodes();  /// поиск свободного узла
};

#endif // DATASTORAGEPROCESSING_H
