#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H
#include "constants.h"
#include "constants.h"
#include "abstractoutput.h"
//#include <QTextStream>
#include <QBuffer>

class ConsoleInput : public AbstractOutput
{
public:
    ConsoleInput(const ConsoleActions&);


    void dataOutput(QSharedPointer<QByteArray>&) override final;

private:
    QByteArray m_ba;

};

#endif // CONSOLEINPUT_H
