#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QVector>
#include <list>
#include <QByteArray>
#include <QTextStream>
#include <QDataStream>

class Server : public QTcpServer
{
	Q_OBJECT
public:
	Server(quint16 port = 0);
	void SendMessageToNode(QTcpSocket* socket, QByteArray& msg);

private:
	QTcpSocket* m_tempSocket;
	QSharedPointer<QByteArray> m_dataStorage;
	quint64 m_waitedBytes;
	bool m_isAwaitingAdditionalData;
private:
	void InitializeSocket(QTcpSocket* socket);
	void ReadDataFromTcp(QDataStream* stream, const qint64 bytesAvailable);

signals:
	void signalSendDataToSerializer(QSharedPointer<QByteArray> ptrMsg);
	void signalSocketDisconnected(QTcpSocket* socketForDisconnect);
	void signalSocketConnected(QTcpSocket* socketForConnect, QHostAddress ip4, quint16 port);

public slots:
	void incomingConnection(qintptr socketDescriptor);
	void slotReadyRead();
	void slotConnectSocket(QHostAddress ip4, quint16 port);
	//void slotCheckTcpConnections();
};