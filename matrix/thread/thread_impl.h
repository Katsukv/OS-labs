#pragma once
#include "matrix.h"

namespace Thread {
    Matrix multiply(const Matrix& a, const Matrix& b, size_t blockSize);
}