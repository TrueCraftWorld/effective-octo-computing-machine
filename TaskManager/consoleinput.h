#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H
#include "constants.h"
#include <QTextStream>
#include <QBuffer>

class ConsoleInput : public QTextStream
{
public:
    ConsoleInput(const ConsoleActions&);


private:
//    QByteArray m_array;
    QBuffer m_buf;

};

#endif // CONSOLEINPUT_H
