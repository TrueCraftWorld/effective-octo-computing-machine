#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

#include <QObject>

class NetworkLayer : public QObject
{
    Q_OBJECT
public:
    explicit NetworkLayer(QObject *parent = nullptr);

signals:

};

#endif // NETWORKLAYER_H
