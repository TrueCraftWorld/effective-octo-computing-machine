
/**
 *   \file     compute_core.cpp
 *   \version  0.01
 *   \date     2024.05.12
 */

#include "compute_core.h"
#include "element.h"
#include "commands.h"
#include <QDataStream>
#include <QStack>


void ComputeCore::compute(QVector<double>& data, QByteArray &formula_ba)
{
    if (data.isEmpty()) return;
    if (formula_ba.isEmpty()) return;
    QDataStream formula(&formula_ba, QIODevice::ReadWrite);
    formula.device()->reset();
    QStack<Element> stack;
    quint8 op = START;

    while (op != END)
    {
        formula >> op;

        Element xe;
        double xd;

        switch (op)
        {
            case REG:
                formula >> xd;
                stack.push({ xd });

                break;

            case ARR:
                stack.push({ &data[0], &data[data.size() - 1]});

                break;

            case ADD:
                xe = stack.pop();
                stack.top() += xe;

                break;

            case SUB:
                xe = stack.pop();
                stack.top() -= xe;

                break;

            case MUL:
                xe = stack.pop();
                stack.top() *= xe;

                break;

            case POW:
                xe = stack.pop();
                stack.top().ePow(xe);

                break;

            case SIN:
                stack.top().eSin();

                break;

            case COS:
                stack.top().eCos();

                break;

            case TAN:
                stack.top().eTan();

                break;

            case CTAN:
                stack.top().eCtan();

                break;
        }
    }
}
