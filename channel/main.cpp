#include <iostream>
#include "Number.h"
#include "Vector.h"

int main() {
    std::cout << "Testing Number library:" << std::endl;
    Number a = CreateNumber(5.0);
    Number b = CreateNumber(3.0);

    std::cout << "a = ";
    a.print();
    std::cout << std::endl;

    std::cout << "b = ";
    b.print();
    std::cout << std::endl;

    std::cout << "a + b = ";
    (a + b).print();
    std::cout << std::endl;

    std::cout << "a - b = ";
    (a - b).print();
    std::cout << std::endl;

    std::cout << "a * b = ";
    (a * b).print();
    std::cout << std::endl;

    std::cout << "a / b = ";
    (a / b).print();
    std::cout << std::endl;

    std::cout << "\nTesting global numbers:" << std::endl;
    std::cout << "NumberZero = ";
    NumberZero.print();
    std::cout << std::endl;

    std::cout << "NumberOne = ";
    NumberOne.print();
    std::cout << std::endl;

    std::cout << "\nTesting Vector library:" << std::endl;
    Vector v1(3.0, 4.0);
    Vector v2(1.0, 2.0);

    std::cout << "v1 = ";
    v1.print();
    std::cout << std::endl;

    std::cout << "v2 = ";
    v2.print();
    std::cout << std::endl;

    std::cout << "v1 + v2 = ";
    v1.add(v2).print();
    std::cout << std::endl;

    std::cout << "Length of v1 = ";
    v1.length().print();
    std::cout << std::endl;

    std::cout << "Angle of v1 = ";
    v1.angle().print();
    std::cout << std::endl;

    std::cout << "\nTesting global vectors:" << std::endl;
    std::cout << "g_origin = ";
    g_origin.print();
    std::cout << std::endl;

    std::cout << "g_one_one = ";
    g_one_one.print();
    std::cout << std::endl;

    return 0;
}