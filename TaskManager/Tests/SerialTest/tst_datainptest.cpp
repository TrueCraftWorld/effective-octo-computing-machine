#include <QtTest>
#include <QObject>
#include <QSharedPointer>
#include <QByteArray>
//#include "tst_datainptest.h"
#include "../../serializer.h"
#include "../../constants.h"
// add necessary includes here

class dataInpTest : public QObject
{
    Q_OBJECT

public:
    dataInpTest();
    ~dataInpTest();

private slots:
    void testDataInput();
    void formulaInput();
private:
    SerialiZer m_serial;
};

dataInpTest::dataInpTest()
{

}

dataInpTest::~dataInpTest()
{

}

void dataInpTest::testDataInput()
{
    QObject::connect(&m_serial, &SerialiZer::messageReady, this, [this](QSharedPointer<QByteArray> msg)
    {

        if (msg->at(0) == DATA_INFO) {
            QByteArray awaitedInfo = QByteArrayLiteral("\xA0\n5\n");
            QCOMPARE(*msg, awaitedInfo);
        } else {
            QByteArray awaitedData = QByteArrayLiteral("\xD0\n1\n11\n111\n1111\n11111\n");
            QCOMPARE(*msg, awaitedData);
        }
        m_serial.disconnect();
    });
    QByteArray input = QByteArrayLiteral("5\n1\n11\n111\n1111\n11111");
    QTextStream text(&input, QIODevice::ReadWrite);
    m_serial.processDataInput(text);

}

void dataInpTest::formulaInput()
{
    QObject::connect(&m_serial, &SerialiZer::messageReady, this, [this](QSharedPointer<QByteArray> msg)
    {

        if (msg->at(0) == FORMULA) {
            QByteArray awaitedInfo = QByteArrayLiteral("\xCA\n\x01\n100\n\x02\x05\n");
            QCOMPARE(*msg, awaitedInfo);
        } else {
//            QFAIL()
        }
        m_serial.disconnect();
    });
    QByteArray input = QByteArrayLiteral("100\nX\n*");
    QTextStream text(&input, QIODevice::ReadWrite);
    m_serial.processDataInput(text);
}

QTEST_APPLESS_MAIN(dataInpTest)

#include "tst_datainptest.moc"
