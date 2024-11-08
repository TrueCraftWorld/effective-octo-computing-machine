#include "fileinput.h"
#include <QIODevice>
#include <QDebug>
#include <QBuffer>
#include <QPointer>
#include <iostream>

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
            bool isFormula = fileDescript.contains("formula");
            if (!isFormula) *this << fileDescript.toLongLong();
            *this << quint32();
            while (!m_file->atEnd()) {
                QString tmp = m_file->readLine();
                bool isOk = false;
                double tmpDouble = tmp.toDouble(&isOk);
                if (isFormula) {
                    *this << tmp;
                } else {
                    if (isOk) {
                            *this << tmpDouble;
                    } else {
                        *this << tmp;
                    }
                }
            }
        }
        m_file->close();
//        device()->reset();
    }

}

FileInput::~FileInput()
{
    m_file->close();
}

void FileInput::dataOutput(QSharedPointer<QByteArray> inp)
{
    m_file->close();

    if (m_file->open(QIODevice::ReadWrite)) {
        QDataStream tmp(inp.get(), QIODevice::ReadWrite);
        quint8 temp;
        tmp >> temp;
        while (!tmp.atEnd()) {
            double val;
            tmp >> val;
//            std::cout << val << std::endl;
            m_file->write(QString("%1\n").arg(val).toLatin1());
        }
        m_file->close();
    }
}
