#include "winapi_impl.h"

#ifdef _WIN32
#include <windows.h>
#include <vector>

namespace {
    DWORD WINAPI multiplyBlock(LPVOID lpParam) {
        ThreadData* data = static_cast<ThreadData*>(lpParam);

        for (size_t i = data->startRow; i < data->endRow; ++i) {
            for (size_t j = 0; j < data->b->getCols(); ++j) {
                double sum = 0.0;
                for (size_t k = 0; k < data->a->getCols(); ++k) {
                    sum += data->a->at(i, k) * data->b->at(k, j);
                }
                data->result->at(i, j) = sum;
            }
        }

        return 0;
    }
}

namespace WinAPI {
    Matrix multiply(const Matrix& a, const Matrix& b, size_t blockSize) {
        return Matrix::multiplyParallel(a, b, blockSize, multiplyBlock);
    }
}
#endif