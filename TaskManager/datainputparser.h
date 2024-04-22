#ifndef DATAINPUTPARSER_H
#define DATAINPUTPARSER_H

#include <QObject>


class DataInputParser : public QObject
{
    Q_OBJECT
public:
    explicit DataInputParser(QObject *parent = nullptr);
    void processFile(QFile&);
signals:
    void dataReady(QByteArray&);
private:
    QByteArray m_data;
};

#endif // DATAINPUTPARSER_H
