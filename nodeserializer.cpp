#include "nodeserializer.h"
#include <QSharedPointer>

void NodeSerializer::slotSerializeDataPrep()
{
}

void NodeSerializer::slotSerializeDataModified()
{
}

void NodeSerializer::slotSerializeNodeDataTcp(double tempMips, quint32 tempPriority, quint16 tempPort)
{

}

void NodeSerializer::slotDeserializeMessage(QTcpSocket* socket, QSharedPointer<QByteArray> ptrMsg)
{
	quint64 sizeOfData;
	QSharedPointer<QVector<double>> ptrList(new QVector<double>());
	double temp;
	double tempMips;  /// �������������� ��������
	quint32 tempPriority;  /// ���������
	QHostAddress tempIp;
	quint16 tempPort;

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
		emit signalDataInfo(sizeOfData);
		break;

	case PKG_DATAPREP:
		streamMsg >> sizeOfData;
		emit signalDataPrep(sizeOfData);
		break;

	case PKG_FORMULA:
		emit signalFormula(ptrMsg);
		break;
	case PKG_DATAARRAY:
		while (!streamMsg.atEnd())
		{
			streamMsg >> temp;
			(*ptrList).push_back(temp);
		}
		emit signalDataArray(ptrList);

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
