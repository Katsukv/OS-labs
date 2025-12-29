#pragma once
#include <cmath>
#include <iostream>
#include "Number.h"

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif

class VECTOR_API Vector {
private:
    Number m_x;
    Number m_y;

public:
    Vector();
    Vector(const Number& x, const Number& y);
    Vector(double x, double y);

    Number getX() const;
    Number getY() const;

    Number length() const;
    Number angle() const;

    Vector add(const Vector& other) const;

    void print() const;
};

extern "C" {
    VECTOR_API extern Vector g_origin;   // (0, 0)
    VECTOR_API extern Vector g_one_one;  // (1, 1)
}