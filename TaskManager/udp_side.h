#ifndef UDP_SIDE_H
#define UDP_SIDE_H

#include <QObject>

class UDP_Side : public QObject
{
    Q_OBJECT
public:
    explicit UDP_Side(QObject *parent = nullptr);

signals:

};

#endif // UDP_SIDE_H
