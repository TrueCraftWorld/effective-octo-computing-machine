
/**
 *   \file     power_meter.h
 *   \version  0.01
 *   \date     2024.05.12
 */

#ifndef POWER_METER_H
#define POWER_METER_H

#include <QObject>


class PowerMeter : public QObject
{
    Q_OBJECT
public:
    explicit PowerMeter(QObject *parent = nullptr);
    double power_check();

signals:

private:
    QVector<double>& prep_data(QVector<double>&);
    QByteArray prep_formula();
    double m_power = -1;
};

#endif // POWER_METER_H
