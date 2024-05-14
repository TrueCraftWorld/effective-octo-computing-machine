#include "nodeserializer.h"
#include <QSharedPointer>

void NodeSerializer::slotSerializeDataPrep()
{
}

void NodeSerializer::slotSerializeDataModified()
{
}

void NodeSerializer::slotDeserializeMessage(QSharedPointer<QByteArray> ptrMsg)
{
	quint64 sizeOfData;
	QSharedPointer<QVector<double>> ptrList(new QVector<double>());
	double temp;

	QDataStream streamMsg(*ptrMsg);
	unsigned char op;
	streamMsg >> op;

	switch (op)
	{

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
		emit signalDataModified(ptrList);
        break;
	}
}
