
/**
 *   \file     element.h
 *   \version  0.01
 *   \date     2024.05.12
 */

#ifndef ELEMENT_H
#define ELEMENT_H
/// <summary>
/// An abstract element that is used in the stack. It can be either a constant double or a reference to an array with values to which functions should be applied.
/// </summary>
///
/// <param name="m_ptr">Pointer to the beginning of the input data array </param>
/// <param name="m_end">Pointer to the ending of the input data array </param>
/// <param name="m_val"> Value </param>
/// <param name="m_isArray">Flag to indicate whether the given element is an array or not. According to this flag, operations will be processed differently </param>
class Element
{
    // TODO: Too much variables in element just for contain double as value and double as array. Which pattern I need to use to fix it? Or it's just fine: we don't send this data to Internet.
    double* m_ptr = nullptr;
    double* m_end = nullptr;
    double m_val = 0.;
    bool m_isArray = false;

    double* get();

public:

    /// <summary>
    /// Constructor for array-element
    /// </summary>
    /// <param name="ptr"> Pointer to the beginning of the input data array </param>
    /// <param name="end"> Pointer to the ending of the input data array </param>
    Element(double* ptr, double* end);

    /// <summary>
    /// Constructor for value-element
    /// </summary>
    /// <param name="val"> Value </param>
    Element(double val);

    /// <summary>
    /// Constructor for empty-element. Needed ONLY for initialization of array with them!
    /// </summary>
    Element() = default;

    /// <summary>
    /// Check if the element is an array
    /// </summary>
    /// <returns> Return "true" if it is array and "false" otherwize </returns>
    bool isArray() const;

    /// <summary>
    /// If element is array, return pointer to the beggining of array. If it is a value, return pointer on value
    /// </summary>
    /// <returns>Pointer on m_ptr or m_val</returns>

    /// <returns> m_ptr - Pointer to the beginning of the input data array </returns>
    double* beginptr() { return m_ptr; }

    /// <returns> m_ptr - Pointer to the ending of the input data array </returns>
    double* endptr() { return m_end; }

    //  [](double& lhs, double& rhs){ return lhs += rhs; }

    /// <summary>
    /// Template for binary operations. Applies a lambda to element.
    /// </summary>
    /// <param name="rhs">Element to which the result will be written. Do it differently depending on element types (array or value)</param>
    /// <param name="op">Lambda, [](double(ref) lhs, double(ref) rhs){  return lhs += rhs; }</param>
    template<typename Predicate>
    void BinaryOperation(Element& rhs, Predicate op)
    {
        if (!m_isArray && !rhs.isArray())
        {
            op(m_val, *rhs.get());
        }
        else if (m_isArray && rhs.isArray())
        {
            for (double* i = m_ptr, *end = m_end + 1; i != end; ++i) {
                op(*i, *i);
            };
        }
        else if (m_isArray)
        {
            for (double* i = m_ptr, *end = m_end + 1; i != end; ++i) {
                op(*i, *rhs.get());
            };
        }
        else
        {
            for (double* i = rhs.beginptr(), *end = rhs.endptr() + 1; i != end; ++i) {
                op(*i, m_val);
            };
            *this = rhs;
        }
    }

    void operator+=(Element& rhs);
    void operator-=(Element& rhs);
    void operator*=(Element& rhs);
    void operator/=(Element& rhs);
    void ePow(Element& rhs);

    template<typename Predicate>
    void UnaryOperation(Predicate op)
    {
        if (m_isArray)
        {
            for (double* i = m_ptr, *end = m_end + 1; i != end; ++i) {
                *i = op(*i);
            };
        }
        else
        {
            *get() = op(*get());
        }
    }

    void eSin();
    void eCos();
    void eTan();
    void eCtan();
};

#endif // ELEMENT_H
