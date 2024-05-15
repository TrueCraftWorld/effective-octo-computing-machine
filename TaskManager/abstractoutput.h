#ifndef ABSTRACTOUTPUT_H
#define ABSTRACTOUTPUT_H
//#include <QTextStream>
#include <QDataStream>

class AbstractOutput : public QDataStream
{
public:
    virtual void dataOutput(QSharedPointer<QByteArray>&) = 0;
};

#endif // ABSTRACTOUTPUT_H
