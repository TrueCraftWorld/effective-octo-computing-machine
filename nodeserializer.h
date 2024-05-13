#pragma once
#include <QObject>
#include <QDataStream>
#include <QSharedPointer>
#include <list>
#include <QVector>

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
	void slotDeserializeMessage(QSharedPointer<QByteArray> ptrMsg);
	void slotSerializeDataPrep();
	void slotSerializeDataModified();
signals:
	void signalDataInfo(quint64 dataAmount);
	void signalDataPrep(quint64 dataWaiting);
	void signalFormula(QSharedPointer<QByteArray> ptrFormula);
	void signalDataArray(QSharedPointer<QVector<double>> values);
	void signalDataModified(QSharedPointer<QVector<double>> values);
private:

};