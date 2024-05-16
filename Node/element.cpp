
/**
 *   \file     element.cpp
 *   \version  0.01
 *   \date     2024.05.12
 */

#include "element.h"

#include <cmath>


Element::Element(double* ptr, double* end) : m_isArray(true), m_ptr(ptr), m_end(end)
{}

Element::Element(double val) : m_isArray(false), m_val(val)
{}

bool Element::isArray() const
{
    return m_isArray;
}

double* Element::get()
{
    return m_isArray ? m_ptr : &m_val;
}

void Element::operator+=(Element& rhs)
{
    BinaryOperation(rhs, [] (double& lhs, double& rhs) { return lhs += rhs; } );
}

void Element::operator-=(Element& rhs)
{
    BinaryOperation(rhs, [] (double& lhs, double& rhs) { return lhs -= rhs; } );
}

void Element::operator*=(Element& rhs)
{
    BinaryOperation(rhs, [] (double& lhs, double& rhs) { return lhs *= rhs; } );
}

void Element::operator/=(Element& rhs)
{
    BinaryOperation(rhs, [](double& lhs, double& rhs) { return lhs /= rhs; });
}

void Element::ePow(Element& rhs)
{
    BinaryOperation(rhs, [] (double& lhs, double& rhs) { return lhs = pow(lhs, rhs); } );
}

void Element::eSin()
{
    UnaryOperation([] (double& val) { return sin(val); } );
}

void Element::eCos()
{
    UnaryOperation([] (double& val) { return cos(val); } );
}

void Element::eTan()
{
    UnaryOperation([] (double& val) { return tan(val); } );
}

void Element::eCtan()
{
    UnaryOperation([] (double& val) { return 1 / tan(val); } );
}
