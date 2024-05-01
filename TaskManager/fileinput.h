#ifndef FILEINPUT_H
#define FILEINPUT_H

#include <QObject>

class FileInput : public QObject
{
    Q_OBJECT
public:
    explicit FileInput(QObject *parent = nullptr);

signals:

};

#endif // FILEINPUT_H
