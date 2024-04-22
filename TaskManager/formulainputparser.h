#ifndef FORMULAINPUTPARSER_H
#define FORMULAINPUTPARSER_H

#include <QObject>

class FormulaInputParser : public QObject
{
    Q_OBJECT
public:
    explicit FormulaInputParser(QObject *parent = nullptr);
    void processFormulaFile(QFile&);
signals:
    void formulaReady();
private:
    QByteArray m_action;
};

#endif // FORMULAINPUTPARSER_H
