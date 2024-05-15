#pragma once
#include <QObject>
#include <QDataStream>
#include <QSharedPointer>
#include <QHostAddress>
#include <list>
#include <QVector>
#include <QTcpSocket>

constexpr unsigned char PKG_NODEDATATCP = 0x90;
constexpr unsigned char PKG_DATAINFO = 0xA0;
constexpr unsigned char PKG_DATAPREP = 0xB0;
constexpr unsigned char PKG_FORMULA = 0xC0;
constexpr unsigned char PKG_DATAARRAY = 0xD0;
constexpr unsigned char PKG_DATAMODIFIED = 0xE0;
constexpr unsigned char PKG_FLAG_CHAR = 0x00;
constexpr unsigned char PKG_FLAG_DOUBLE = 0x01;

class NodeSerializer : public QObject
{
	Q_OBJECT
public slots:

	void slotDeserializeMessage(QTcpSocket* socket, QSharedPointer<QByteArray> ptrMsg);
signals:
	// Сигналы для приёма извне
	void signalNodeDataTcp(double tempMips, quint32 tempPriority, quint16 tempPort, QTcpSocket* tempSocket);
	void signalDataInfo(QTcpSocket* socketClient, quint64 dataAmount);
	void signalDataPrep(QTcpSocket* socketSelectedNode, quint64 dataWaiting);
	void signalFormula(QTcpSocket* socketSelectedNode, QSharedPointer<QByteArray> ptrFormula);
	void signalDataArray(QTcpSocket* socketSelectedNode, QSharedPointer<QVector<double>> values);
	void signalDataModified(QTcpSocket* socketSimpleNode, QVector<double>& values);
private:
	
};
