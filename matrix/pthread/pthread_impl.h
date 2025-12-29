#pragma once
#include "matrix.h"

namespace PThread {
    Matrix multiply(const Matrix& a, const Matrix& b, size_t blockSize);
}