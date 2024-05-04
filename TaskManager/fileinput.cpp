#include "fileinput.h"
#include <QIODevice>
#include <QDebug>
#include <QBuffer>

FileInput::FileInput(QString filePath)
{
    m_file = new QFile(filePath);
    if (m_file->exists()) {
        if (m_file->open(QIODevice::ReadOnly)) {
            m_file->seek(0);
            setDevice(m_file);
        }
    }
}

FileInput::~FileInput()
{
    m_file->close();
}
