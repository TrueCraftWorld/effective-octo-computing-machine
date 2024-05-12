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
};

#endif // FILEINPUT_H
