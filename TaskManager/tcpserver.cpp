#include "tcpserver.h"
#include <QStack>
#include <QFile>

//#include <fstream>
#include <cmath>

constexpr quint16 KEY_PROGRAM = 132;

constexpr unsigned char PKG_DATAINFO = 0xA0;
constexpr unsigned char PKG_DATAPREP = 0xB0;
constexpr unsigned char PKG_FORMULA = 0xC0;
constexpr unsigned char PKG_DATAARRAY = 0xD0;
constexpr unsigned char PKG_DATAMODIFIED = 0xE0;
constexpr unsigned char PKG_CONNECTIONCHECK = 0xF0;

constexpr char PADDING = '\n';

Server::Server(quint16 port)
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

void Server::SendMessageToNode(QTcpSocket* socket, QSharedPointer<QByteArray> msg)
{
    if (socket == nullptr) return;
    QByteArray temp_msg;
    temp_msg.clear();


//    temp_msg.push_back(PADDING);


    temp_msg.push_back("132");
    temp_msg.push_back(PADDING);
    temp_msg.push_back(QString("%1").arg(msg->size()).toLatin1());
    temp_msg.push_back(PADDING);
    temp_msg.push_back(*msg);

//	if (socket != nullptr)
//	{
    socket->write(temp_msg);
//        socket->
//	}
}

void Server::slotReadyRead()
{
    qDebug() << "recieve";
    m_tempSocket = (QTcpSocket*)sender();
    quint32 bytesArrived = m_tempSocket->bytesAvailable();
    quint16 keyProgram = 0;
    QTextStream streamIn(m_tempSocket);
    if (m_isAwaitingAdditionalData) {
        //WHAT TO DO if there is start of new message in the end of
        //branch of continious accepting
    } else if (bytesArrived < 10) {
        return;
    } else {

        streamIn >> keyProgram;
        if (keyProgram != KEY_PROGRAM)
            return;
//        }
//        else {
            streamIn >> m_waitedBytes;
            qint64 diff =  (bytesArrived - (3 + QString("%1").arg(m_waitedBytes).size()));
            while (diff > 0) {
                QSharedPointer<QByteArray> tmp (new QByteArray());
                tmp->append(streamIn.read(m_waitedBytes).toLatin1());
                emit signalSendDataToSerializer(tmp);
                streamIn >> keyProgram;
                if (keyProgram != KEY_PROGRAM)
                    return;
                  streamIn >> m_waitedBytes;
            }
            if (diff < 0) { //accept less then needed
//                m_isAwaitingAdditionalData = true;
//                m_dataStorage = (new QByteArray());
//                streamIn >> *m_dataStorage;
//                m_waitedBytes -= abs(diff);
//                return; //

                //DEMO: should be working but there is a problem above so commented out
            } else if (diff == 0) {
                //            emit signalSendDataToSerializer(QSharedPointer<QByteArray>(m_dataStorage));
                //            m_dataStorage = nullptr;
                QSharedPointer<QByteArray> tmp (new QByteArray());
                streamIn >> *tmp;
                emit signalSendDataToSerializer(tmp);
            }
    }


//    QTextStream streamIn(m_tempSocket);
//	unsigned char op;
//    QSharedPointer<QByteArray> tmp (new QByteArray(streamIn.device()->readAll()));
//    *tmp = (QByteArray(streamIn.device()->readAll()));
//    signalSendDataToSerializer(tmp);
//
//    qDebug() << (*tmp);
//	m_tempSocket = (QTcpSocket*)sender();
//	QDataStream streamIn(m_tempSocket);
//	unsigned char op;

//	// If not all the data came in at once, expect an additional packet
//    if (m_isAwaitingAdditionalData) {
//		ReadDataFromTcp();
//        if (m_waitedBytes == 0) {
//            emit signalSendDataToSerializer(QSharedPointer<QByteArray>(m_dataStorage));
//            m_dataStorage = nullptr;
//			m_isAwaitingAdditionalData = false;
//		}
//		return;
//    } else if (m_tempSocket->bytesAvailable() < 10) {
//		return;
//	}

//	quint16 keyProgram;
//	streamIn >> keyProgram;

//	// Check if it is our programm package
//    if (keyProgram != KEY_PROGRAM) {
//		return;
//    } else  {
//		streamIn >> m_waitedBytes;

//		// At first reading m_waitedBytes == 0 only if it is connectionCheck
//		if (m_waitedBytes == 0)
//			return;

//		ReadDataFromTcp();
		
//        if (m_waitedBytes != 0) {
//			m_isAwaitingAdditionalData = true;
//        } else {
//            emit signalSendDataToSerializer(QSharedPointer<QByteArray>(m_dataStorage));
//            m_dataStorage = nullptr;
//		}
//	}
}

void Server::ReadDataFromTcp() 
{
    if (m_dataStorage == nullptr) m_dataStorage = new QByteArray();
    quint64 minForRead = qMin<>(m_waitedBytes, quint64(m_tempSocket->bytesAvailable()));
    m_dataStorage->append(m_tempSocket->read(minForRead)); // BUG, TODO: Change m_tempSocket to QDataStream
	m_waitedBytes -= minForRead;
}


void Server::slotConnectSocket(QHostAddress ip4, quint16 port)
{
	m_tempSocket = new QTcpSocket(this);
	m_tempSocket->connectToHost(ip4, port);
	InitializeSocket(m_tempSocket);
	emit signalSocketConnected(m_tempSocket, ip4, port);
	qDebug() << "Connecting tcp to: " << ip4 << ' ' << port;
}


void Server::incomingConnection(qintptr socketDescriptor)
{
	m_tempSocket = new QTcpSocket(this);
	m_tempSocket->setSocketDescriptor(socketDescriptor);
	InitializeSocket(m_tempSocket);
	emit signalSocketConnected(m_tempSocket, m_tempSocket->peerAddress(), m_tempSocket->peerPort());
	qDebug() << "TCP, client connected: " << socketDescriptor;
}

void Server::InitializeSocket(QTcpSocket* socket)
{
	connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
	connect(socket, &QTcpSocket::disconnected,
		[this, socket]
		{
			qDebug() << "TCP, disconnected: " << socket->socketDescriptor();
			emit signalSocketDisconnected(socket);
		});
}
