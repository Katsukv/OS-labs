#pragma once
#include <iostream>

class Number {
private:
    double m_value;

public:
    Number();
    Number(double value);

    double getValue() const;
    void setValue(double value);

    Number operator+(const Number& other) const;
    Number operator-(const Number& other) const;
    Number operator*(const Number& other) const;
    Number operator/(const Number& other) const;

    void print() const;
};

extern Number NumberZero;
extern Number NumberOne;

Number CreateNumber(double value);