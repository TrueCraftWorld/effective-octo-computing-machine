#ifndef DATAOUTPUTREADER_H
#define DATAOUTPUTREADER_H

#include <QObject>
#include <QFile>

class DataOutputReader : public QObject
{
    Q_OBJECT
public:
    explicit DataOutputReader(QObject *parent = nullptr);
    void saveToFile(QFile &);
    void setData(QByteArray& );
signals:
private:
    QByteArray m_data;
};

#endif // DATAOUTPUTREADER_H
