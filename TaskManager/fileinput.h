#ifndef FILEINPUT_H
#define FILEINPUT_H
#include "abstractoutput.h"
#include <QObject>
#include <QFile>
#include <QTextStream>

class FileInput : public AbstractOutput
{
public:
    FileInput(QString filePath);
    ~FileInput();
    void dataOutput(QSharedPointer<QByteArray>) override;
signals:
    QFile *m_file;
//    QByteArray m_arr;
};

#endif // FILEINPUT_H
