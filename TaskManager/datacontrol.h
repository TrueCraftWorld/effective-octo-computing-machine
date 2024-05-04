#ifndef DATACONTROL_H
#define DATACONTROL_H

#include <QTextStream>
#include <QDataStream>
#include <QObject>

class DataControl : public QObject
{
    Q_OBJECT
public:
    DataControl();
    void processFile(QTextStream & input);
signals:
    void dataReady(QByteArray& , char);
private:
    QByteArray m_data;
    qint32 m_linesCount = 0;
};

#endif // DATACONTROL_H
