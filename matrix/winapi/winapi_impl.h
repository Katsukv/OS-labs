#pragma once
#include "matrix.h"

#ifdef _WIN32
namespace WinAPI {
    Matrix multiply(const Matrix& a, const Matrix& b, size_t blockSize);
}
#endif