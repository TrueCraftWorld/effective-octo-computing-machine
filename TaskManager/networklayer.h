#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>

class NetworkLayer : public QObject
{
    Q_OBJECT
public:
    explicit NetworkLayer(QObject *parent = nullptr);

    void connectTo(const QHostAddress&, quint16);
    void sendData(const QByteArray&);

signals:
    void dataReady(QByteArray& );

private:
    QTcpSocket m_tcpSocket;
    QByteArray recievedData;
};

#endif // NETWORKLAYER_H
