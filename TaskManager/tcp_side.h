#ifndef TCP_SIDE_H
#define TCP_SIDE_H

#include <QObject>

class TCP_Side : public QObject
{
    Q_OBJECT
public:
    explicit TCP_Side(QObject *parent = nullptr);

signals:

};

#endif // TCP_SIDE_H
