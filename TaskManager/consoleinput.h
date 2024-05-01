#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

#include <QObject>

class ConsoleInput : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleInput(QObject *parent = nullptr);

signals:

};

#endif // CONSOLEINPUT_H
