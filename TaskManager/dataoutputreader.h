#ifndef DATAOUTPUTREADER_H
#define DATAOUTPUTREADER_H

#include <QObject>
#include <QFile>

class DataOutputReader : public QObject
{
    Q_OBJECT
public:
    explicit DataOutputReader(QObject *parent = nullptr);
    void setFile(QFile *);
    void setData(QByteArray& );

signals:
private:
    void saveToFile();
    QByteArray m_data;
    QFile * m_file;
};

#endif // DATAOUTPUTREADER_H
