#include "thread_impl.h"
#include <thread>
#include <vector>

namespace {
    void multiplyBlock(const Matrix* a, const Matrix* b, Matrix* result,
                      size_t startRow, size_t endRow) {
        for (size_t i = startRow; i < endRow; ++i) {
            for (size_t j = 0; j < b->getCols(); ++j) {
                double sum = 0.0;
                for (size_t k = 0; k < a->getCols(); ++k) {
                    sum += a->at(i, k) * b->at(k, j);
                }
                result->at(i, j) = sum;
            }
        }
    }
}

namespace Thread {
    Matrix multiply(const Matrix& a, const Matrix& b, size_t blockSize) {
        if (a.getCols() != b.getRows()) {
            throw std::invalid_argument("Matrix dimensions mismatch");
        }

        Matrix result(a.getRows(), b.getCols());

        size_t numThreads = (a.getRows() + blockSize - 1) / blockSize;
        std::vector<std::thread> threads;
        threads.reserve(numThreads);

        for (size_t i = 0; i < numThreads; ++i) {
            size_t startRow = i * blockSize;
            size_t endRow = std::min(startRow + blockSize, a.getRows());

            threads.emplace_back(multiplyBlock, &a, &b, &result, startRow, endRow);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        return result;
    }
}