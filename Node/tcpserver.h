#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QVector>
#include <list>
#include <QByteArray>
#include <QTextStream>
#include <QDataStream>
#include "node_info.h"

class TcpModule : public QTcpServer
{
	Q_OBJECT
public:
	TcpModule(QObject* parent, quint16 port = 0);
	void SendMessageToNode(QTcpSocket* socket, QByteArray& msg);

private:
	QTcpSocket* m_tempSocket;
	QSharedPointer<QByteArray> m_dataStorage;
	quint32 m_waitedBytes;
	bool m_isAwaitingAdditionalData;
private:
	void InitializeSocket(QTcpSocket* socket);
	void ReadDataFromTcp(QDataStream* stream, qint32& bytesAvailable);

signals:
	void signalSendDataToSerializer(QTcpSocket* socket, QSharedPointer<QByteArray> ptrMsg);
	void signalSocketDisconnected(QTcpSocket* socketForDisconnect);
	void signalSocketConnected(QTcpSocket* socketForConnect, QHostAddress ip4, quint16 port);
	void signalNodeDataTcp(QTcpSocket* socketForConnect);

public slots:
	void incomingConnection(qintptr socketDescriptor);
	void slotReadyRead();
	void slotConnectSocket(NodeID node_id);
	//void slotCheckTcpConnections();
};