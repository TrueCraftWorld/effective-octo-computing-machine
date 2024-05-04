#ifndef FILEINPUT_H
#define FILEINPUT_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class FileInput : public QTextStream
{
public:
    FileInput(QString filePath);
    ~FileInput();
signals:
    QFile *m_file;
//    QByteArray m_arr;
};

#endif // FILEINPUT_H
