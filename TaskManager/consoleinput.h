#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H
#include "constants.h"
#include "constants.h"
#include "abstractoutput.h"
#include <QTextStream>
#include <QBuffer>

class ConsoleInput : public AbstractOutput, public QTextStream
{
public:
    ConsoleInput(const ConsoleActions&);
    void dataOutput(QSharedPointer<QByteArray>) override;

private:
//    QByteArray m_array;
    QBuffer m_buf;

};

#endif // CONSOLEINPUT_H
