#include "tcpserver.h"
#include <QStack>
#include <QFile>
#include <fstream>
#include <QTextStream>
#include <QDataStream>
#include "node_info.h"
#include <cmath>

constexpr quint16 KEY_PROGRAM = 132;

constexpr unsigned char PKG_DATAINFO = 0xA0;
constexpr unsigned char PKG_DATAPREP = 0xB0;
constexpr unsigned char PKG_FORMULA = 0xC0;
constexpr unsigned char PKG_DATAARRAY = 0xD0;
constexpr unsigned char PKG_DATAMODIFIED = 0xE0;
constexpr unsigned char PKG_CONNECTIONCHECK = 0xF0;

TcpModule::TcpModule(quint16 port)
{
	m_isAwaitingAdditionalData = false;
	if (this->listen(QHostAddress::Any, port))
	{
		qDebug() << "start tcp server" << this->serverPort();
	}
	else
	{
		this->listen(QHostAddress::Any, 0);
		qDebug() << "port" << port << "busy, new port" << this->serverPort();
	}
}

void TcpModule::SendMessageToNode(QTcpSocket* socket, QByteArray& msg)
{
	QByteArray temp_msg;
	temp_msg.clear();

	QDataStream msgStream(&temp_msg, QIODevice::WriteOnly);

	msgStream << quint16(KEY_PROGRAM) << msg ;

	if (socket != nullptr && socket->isValid())
	{
		socket->write(temp_msg);
	}
}

void TcpModule::slotReadyRead()
{
	m_tempSocket = (QTcpSocket*)sender();
	QDataStream streamIn(m_tempSocket);
	streamIn.setVersion(QDataStream::Qt_5_15);
	//QTextStream streamIn(m_tempSocket);

	qint32 bytesAvailable = m_tempSocket->bytesAvailable();

	// If not all the data came in at once, expect an additional packet
	if (m_isAwaitingAdditionalData)
	{
		ReadDataFromTcp(&streamIn, bytesAvailable);
		if (m_waitedBytes == 0)
		{
			emit signalSendDataToSerializer(m_dataStorage);
			m_isAwaitingAdditionalData = false;
		}
		return;
	}

	while (bytesAvailable >= 6)
	{
		quint16 keyProgram;
		streamIn >> keyProgram;

		// Check if it is our programm package
		if (keyProgram != KEY_PROGRAM)
		{
			qDebug() << "WARNING: ERROR IN PACKAGE READING";
			return;
		}
		else
		{
			m_dataStorage = QSharedPointer<QByteArray>(new QByteArray());

			streamIn >> m_waitedBytes;
			m_dataStorage->resize(m_waitedBytes);

			ReadDataFromTcp(&streamIn, bytesAvailable);

//			// At first reading m_waitedBytes == 0 only if it is connectionCheck
//			if (m_waitedBytes == 0)
//				continue;

			if (m_waitedBytes != 0)
			{
				m_isAwaitingAdditionalData = true;
			}
			else
			{
				emit signalSendDataToSerializer(m_dataStorage);
			}
		}
	}

}

void TcpModule::ReadDataFromTcp(QDataStream* stream, qint32& bytesAvailable)
{
	quint64 minForRead = qMin<>(m_waitedBytes, quint32(bytesAvailable));
	stream->readRawData(m_dataStorage->data(), minForRead);
	m_waitedBytes -= minForRead;
	bytesAvailable -= minForRead + sizeof(quint16) + sizeof(quint32);
}


void TcpModule::slotConnectSocket(NodeID node_id)
{
	m_tempSocket = new QTcpSocket(this);
	m_tempSocket->connectToHost(node_id.ip, node_id.port);
	InitializeSocket(m_tempSocket);
	emit signalSocketConnected(m_tempSocket, node_id.ip, node_id.port);
	qDebug() << "Connecting tcp to: " << node_id.ip << ' ' << node_id.port;
}

void TcpModule::incomingConnection(qintptr socketDescriptor)
{
	m_tempSocket = new QTcpSocket(this);
	m_tempSocket->setSocketDescriptor(socketDescriptor);
	InitializeSocket(m_tempSocket);
	emit signalSocketConnected(m_tempSocket, m_tempSocket->peerAddress(), m_tempSocket->peerPort());
	qDebug() << "TCP, client connected: " << socketDescriptor;
}

void TcpModule::InitializeSocket(QTcpSocket* socket)
{
	connect(socket, &QTcpSocket::readyRead, this, &TcpModule::slotReadyRead);
	connect(socket, &QTcpSocket::disconnected,
		[this, socket]
		{
			qDebug() << "TCP, disconnected: " << socket->socketDescriptor();
			emit signalSocketDisconnected(socket);
		});
}

/*void Server::slotReadyRead()
{
	QString messageDebug = "132\n14\n11\n11\n22\n22\n33";
	m_tempSocket = (QTcpSocket*)sender();
	QTextStream streamIn(&messageDebug);
	//QTextStream streamIn(m_tempSocket);
	streamIn.setAutoDetectUnicode(false);
	unsigned char op;

	const qint64 bytesAvailable = m_tempSocket->bytesAvailable();

	// If not all the data came in at once, expect an additional packet
	if (m_isAwaitingAdditionalData)
	{
		ReadDataFromTcp(&streamIn, bytesAvailable);
		if (m_waitedBytes == 0)
		{
			emit signalSendDataToSerializer(m_dataStorage);
			m_isAwaitingAdditionalData = false;
		}
		return;
	}
	else if (bytesAvailable < 10)
	{
		return;
	}

	QString tempString;
	streamIn >> tempString;
	quint16 keyProgram = tempString.toInt();

	// Check if it is our programm package
	if (keyProgram != KEY_PROGRAM)
	{
		return;
	}
	else 
	{
		m_dataStorage = QSharedPointer<QByteArray>(new QByteArray());

		streamIn >> m_waitedBytes;

		// At first reading m_waitedBytes == 0 only if it is connectionCheck
		if (m_waitedBytes == 0)
			return;

		streamIn.readLine();
		ReadDataFromTcp(&streamIn, bytesAvailable);
		
		if (m_waitedBytes != 0)
		{
			m_isAwaitingAdditionalData = true;
		}
		else
		{
			emit signalSendDataToSerializer(m_dataStorage);
		}
	}
}

void Server::ReadDataFromTcp(QTextStream *stream, const qint64 bytesAvailable)
{
	quint64 minForRead = qMin<>(m_waitedBytes, quint64(bytesAvailable));
	m_dataStorage.data()->append(stream->read(minForRead).toLatin1());
	m_waitedBytes -= minForRead;
}
*/
