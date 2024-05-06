#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QVector>
#include <QLinkedList>
#include <QByteArray>
#include <QSharedPointer>

class Server : public QTcpServer
{
	Q_OBJECT
public:
	Server(quint16 port = 0);
//	void SendMessageToNode(QTcpSocket* socket, const QByteArray& msg);
    void SendMessageToNode(QTcpSocket* socket, QSharedPointer<QByteArray>);

private:
	QTcpSocket* m_tempSocket;
    QByteArray* m_dataStorage;
	quint64 m_waitedBytes;
	bool m_isAwaitingAdditionalData;
private:
	void InitializeSocket(QTcpSocket* socket);
	void ReadDataFromTcp();

signals:
//    void signalSendDataToSerializer(const QByteArray& msg);
    void signalSendDataToSerializer(QSharedPointer<QByteArray>);
	void signalSocketDisconnected(QTcpSocket* socketForDisconnect);
	void signalSocketConnected(QTcpSocket* socketForConnect, QHostAddress ip4, quint16 port);

public slots:
	void incomingConnection(qintptr socketDescriptor);
	void slotReadyRead();
	void slotConnectSocket(QHostAddress ip4, quint16 port);
	//void slotCheckTcpConnections();
};
