#ifndef ABSTRACTOUTPUT_H
#define ABSTRACTOUTPUT_H
#include <QTextStream>

class AbstractOutput
{
public:
    virtual void dataOutput(QSharedPointer<QByteArray>) = 0;
};

#endif // ABSTRACTOUTPUT_H
