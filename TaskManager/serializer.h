#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QObject>
#include <QTextStream>

class SerialiZer : public QObject
{
    Q_OBJECT
public:
    explicit SerialiZer(QObject *parent = nullptr);
    void acceptData(QTextStream* );
signals:

};

#endif // SERIALIZER_H
