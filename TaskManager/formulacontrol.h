#ifndef FORMULACONTROL_H
#define FORMULACONTROL_H

#include <QObject>
#include <QTextStream>




class FormulaControl : public QObject
{
    Q_OBJECT
public:
    explicit FormulaControl(QObject *parent = nullptr);
    void processFormulaFile(QTextStream &);
signals:
    void formulaReady(QByteArray& , char);
private:
    QByteArray m_action;
};

#endif // FORMULACONTROL_H
