#include "nodeserializer.h"
#include <QSharedPointer>


void NodeSerializer::slotDeserializeMessage(QTcpSocket* socket, QSharedPointer<QByteArray> ptrMsg)
{
	quint64 sizeOfData;
	QSharedPointer<QVector<double>> ptrList(new QVector<double>());
	double temp;
	double tempMips;  /// �������������� ��������
	quint32 tempPriority;  /// ���������
	QHostAddress tempIp;
	quint16 tempPort;
	QSharedPointer<QByteArray> tempFormula(new QByteArray);


	QDataStream streamMsg(*ptrMsg);
	unsigned char op;
	streamMsg >> op;

	switch (op)
	{
	case PKG_NODEDATATCP:
		streamMsg >> tempPort;
		streamMsg >> tempMips;
		streamMsg >> tempPriority;
		emit signalNodeDataTcp(tempMips, tempPriority, tempPort, socket);
		break;
	case PKG_DATAINFO:
		streamMsg >> sizeOfData;
		emit signalDataInfo(socket, sizeOfData);
		break;

	case PKG_DATAPREP:
		streamMsg >> sizeOfData;
		emit signalDataPrep(socket, sizeOfData);
		break;

	case PKG_FORMULA:
		tempFormula->resize(ptrMsg->size() - 1);
		streamMsg.readRawData(tempFormula->data(), ptrMsg->size() - 1);
		emit signalFormula(socket, tempFormula);
		break;
	case PKG_DATAARRAY:
		while (!streamMsg.atEnd())
		{
			streamMsg >> temp;
			(*ptrList).push_back(temp);
		}
		emit signalDataArray(socket, ptrList);
        break;
	case PKG_DATAMODIFIED:
		while (!streamMsg.atEnd())
		{
			streamMsg >> temp;
			(*ptrList).push_back(temp);
		}
		emit signalDataModified(socket, ptrList);
        break;
	}
}

void NodeSerializer::slotSerializeDataPrep(QString selectedNode, quint64 dataWaiting)
{
}

void NodeSerializer::slotSerializeFormula(QString selectedNode, QByteArray)
{
}

void NodeSerializer::slotSerializeDataPrep(QString selectedNode, QVector<double> data)
{
}

void NodeSerializer::slotSerializeDataModified(QString simpleNode, quint64 dataWaiting)
{
}

void NodeSerializer::slotSerializeNodeDataTcp(double tempMips, quint32 tempPriority, quint16 tempPort)
{
}

void NodeSerializer::ParseQStringToIpPort(const QString& str, QHostAddress& ip, quint16& port)
{
	QStringList list = str.split(':');
	ip = QHostAddress(list.at(0));
	port = list.at(1).toInt();
}
