
/**
 *   \file     power_meter.cpp
 *   \version  0.01
 *   \date     2024.05.12
 */

#include "power_meter.h"
#include "commands.h"
#include "loger.h"
#include "compute_core.h"

#include <QTextStream>
#include <QDataStream>
#include <QRandomGenerator>
#include <QSharedPointer>


/**
 *   \brief   Конструктор
 *   \param  *parent - указатель на родительский класс
 *   \retval  Нет
 */
PowerMeter::PowerMeter(QObject *parent)
    : QObject{parent}
{}


/**
 *   \brief   Замер вычислительной мощности узла
 *   \param   Нет
 *   \retval  Вычислительная мощность узла
 */
double PowerMeter::power_check()
{
    if (m_power < 0)
    {
        QVector<double> data_double;
        QByteArray formula = prep_formula();
        prep_data(data_double);

        LogDuration time("Calc");
        ComputeCore::compute(data_double, formula);
        m_power = static_cast<double>(time.timeElapsed());
    }
    return m_power;
}


/**
 *   \brief   Подготовка формулы
 *   \param   Нет
 *   \retval  Формула
 */
QByteArray PowerMeter::prep_formula()
{
    QByteArray formula;
    QTextStream form_stream(&formula, QIODevice::ReadWrite);


    form_stream << 100 << "\n" << "X" << "\n" << "*" << "\n" << 25 << "\n" << "-" << "\n" << 0.333 << "\n" << "/" << "\n" << 111
               << "\n" << "+" << "\n" << "S" << "\n" << 1 << "\n" << "+" << "\n" << "C" << "\n" << "T" << "\n" << "Y" << "\n";
    form_stream << 100 << "\n" << "*" << "\n" << 25 << "\n" << "-" << "\n" << 0.333 << "\n" << "/" << "\n" << 111
               << "\n" << "+" << "\n" << "S" << "\n" << 1 << "\n" << "+" << "\n" << "C" << "\n" << "T" << "\n" << "Y" << "\n";
    form_stream << 100 << "\n" << "*" << "\n" << 25 << "\n" << "-" << "\n" << 0.333 << "\n" << "/" << "\n" << 111
               << "\n" << "+" << "\n" << "S" << "\n" << 1 << "\n" << "+" << "\n" << "C" << "\n" << "T" << "\n" << "Y" << "\n";
    form_stream << 100 << "\n" << "*" << "\n" << 25 << "\n" << "-" << "\n" << 0.333 << "\n" << "/" << "\n" << 111
               << "\n" << "+" << "\n" << "S" << "\n" << 1 << "\n" << "+" << "\n" << "C" << "\n" << "T" << "\n" << "Y" << "\n";
    form_stream << 100 << "\n" << "*" << "\n" << 25 << "\n" << "-" << "\n" << 0.333 << "\n" << "/" << "\n" << 111
               << "\n" << "+" << "\n" << "S" << "\n" << 1 << "\n" << "+" << "\n" << "C" << "\n" << "T" << "\n" << "Y" << "\n";
    form_stream << 100 << "\n" << "*" << "\n" << 25 << "\n" << "-" << "\n" << 0.333 << "\n" << "/" << "\n" << 111
               << "\n" << "+" << "\n" << "S" << "\n" << 1 << "\n" << "+" << "\n" << "C" << "\n" << "T" << "\n" << "Y" << "\n";
    form_stream << 100 << "\n" << "*" << "\n" << 25 << "\n" << "-" << "\n" << 0.333 << "\n" << "/" << "\n" << 111
               << "\n" << "+" << "\n" << "S" << "\n" << 1 << "\n" << "+" << "\n" << "C" << "\n" << "T" << "\n" << "Y" << "\n";
    form_stream << 100 << "\n" << "*" << "\n" << 25 << "\n" << "-" << "\n" << 0.333 << "\n" << "/" << "\n" << 111
               << "\n" << "+" << "\n" << "S" << "\n" << 1 << "\n" << "+" << "\n" << "C" << "\n" << "T" << "\n" << "Y" << "\n";

    form_stream.flush();
    form_stream.reset();
    form_stream.device()->reset();

    //formula readOut
    QByteArray actions;
    QDataStream stream_vm_out(&actions, QIODevice::ReadWrite);
    bool is_double_value;

    while (!form_stream.atEnd())
    {
        QString line;
        form_stream >> line;
        double valueDouble = line.toDouble(&is_double_value);

        if (is_double_value)
        {
            stream_vm_out << REG << valueDouble;
        }
        else
        {
            switch (line[0].toLatin1())
            {
                case '+':
                    stream_vm_out << ADD;

                    break;

                case '-':
                    stream_vm_out << SUB;

                    break;

                case '*':
                    stream_vm_out << MUL;

                    break;

                case '^':
                    stream_vm_out << DIV;

                    break;

                case 'S':
                    stream_vm_out << SIN;

                    break;

                case 'C':
                    stream_vm_out << COS;

                    break;

                case 'T':
                    stream_vm_out << TAN;

                    break;

                case 'Y':
                    stream_vm_out << CTAN;

                    break;

                case 'X':
                    stream_vm_out << ARR;

                    break;
            }
        }
    }

    stream_vm_out << END;
    stream_vm_out.device()->reset();

    return actions; //believe in RVO
}


/**
 *   \brief   Подготовка данных
 *   \param   Нет
 *   \retval  Дынные
 */
QVector<double>& PowerMeter::prep_data(QVector<double>& data_double)
{
    QVector<quint32> data_int;

    data_int.resize(10'000'000);
    data_double.resize(10'000'000);

    QRandomGenerator::global()->fillRange(data_int.data(), data_int.size());

    for (quint32 i = 0; i < data_int.size(); ++i)
    {
        data_double[i] = static_cast<double>(data_int.at(i));
    }

    QVector<quint32>().swap(data_int); //doc said it clears used memory;

    return data_double;
}
