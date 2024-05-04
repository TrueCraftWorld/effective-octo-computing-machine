#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H
#include "constants.h"
#include <QTextStream>

class ConsoleInput : public QTextStream
{
public:
    ConsoleInput(const ConsoleActions&);


private:
    QByteArray m_array;

};

#endif // CONSOLEINPUT_H
