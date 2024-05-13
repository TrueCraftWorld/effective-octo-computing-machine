#include "fileinput.h"
#include <QIODevice>
#include <QDebug>
#include <QBuffer>
#include <QPointer>
//#include <QTextStream>

FileInput::FileInput(QString filePath)
{
    setVersion(QDataStream::Qt_5_15);
    QPointer<QBuffer> dynamicBuff (new QBuffer(&m_ba));
    setDevice(dynamicBuff);
    device()->open(QIODevice::ReadWrite);
    setVersion(QDataStream::Qt_5_15);
    m_file = new QFile(filePath);
    if (m_file->exists()) {
        if (m_file->open(QIODevice::ReadOnly)) {
            m_file->seek(0);
            QString fileDescript = m_file->readLine();
            if (!fileDescript.contains("formula")) *this << fileDescript.toLongLong();
            while (!m_file->atEnd()) {
                QString tmp = m_file->readLine();
                bool isOk = false;
                double tmpDouble = tmp.toDouble(&isOk);
                if (isOk) {
                    *this << tmpDouble;
                } else {
                    *this << tmp;
                }
            }
        }
        m_file->close();
        device()->reset();
    }

}

FileInput::~FileInput()
{
    m_file->close();
}

void FileInput::dataOutput(QSharedPointer<QByteArray> inp)
{
    m_file->close();
//    reset();
    if (m_file->open(QIODevice::ReadWrite)) {
//        m_file->seek(0);
//        setDevice(m_file);
        QDataStream tmp(inp.get(), QIODevice::ReadWrite);
        while (!tmp.atEnd()) {
            double val;
            tmp >> val;
            m_file->write(QString("%1\n").arg(val).toLatin1());
        }
        m_file->close();
//        flush();
    }
}
