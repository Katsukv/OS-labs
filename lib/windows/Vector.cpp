#include "Vector.h"

Vector g_origin(NumberZero, NumberZero);
Vector g_one_one(NumberOne, NumberOne);

Vector::Vector() : m_x(NumberZero), m_y(NumberZero) {}

Vector::Vector(const Number& x, const Number& y) : m_x(x), m_y(y) {}

Vector::Vector(double x, double y) : m_x(Number(x)), m_y(Number(y)) {}

Number Vector::getX() const { return m_x; }

Number Vector::getY() const { return m_y; }

Number Vector::length() const {
    Number x_squared = m_x * m_x;
    Number y_squared = m_y * m_y;
    Number sum = x_squared + y_squared;
    return Number(sqrt(sum.getValue()));
}

Number Vector::angle() const {
    return Number(atan2(m_y.getValue(), m_x.getValue()));
}

Vector Vector::add(const Vector& other) const {
    return Vector(m_x + other.m_x, m_y + other.m_y);
}

void Vector::print() const {
    std::cout << "(";
    m_x.print();
    std::cout << ", ";
    m_y.print();
    std::cout << ")";
}