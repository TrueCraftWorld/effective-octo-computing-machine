#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>

constexpr int MAX_DATA_FRAME = 2000;
constexpr char DATA_IN = 111;
constexpr char DATA_OUT = 222;
constexpr char FORMULA = 78; //AKA 333 OVERFLOW


class NetworkLayer : public QObject
{
    Q_OBJECT
public:
    explicit NetworkLayer(QObject *parent = nullptr);

    void connectTo(const QHostAddress&, quint16);
    void sendData(const QByteArray&, char);
    void socketSetup();
    QByteArray& getData();

signals:
    void dataReady();

private:
    void dataRecieve();
    QTcpSocket* m_tcpSocket;
    QByteArray m_recievedData;
    quint32 m_awaitedSize;
};

#endif // NETWORKLAYER_H
