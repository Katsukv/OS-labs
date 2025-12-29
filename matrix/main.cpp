#include <iostream>
#include <iomanip>
#include "matrix.h"
#include "thread_impl.h"
#include "winapi_impl.h"
#include "pthread_impl.h"

void testMultiplication(const std::string& name,
                       Matrix (*multiplyFunc)(const Matrix&, const Matrix&, size_t),
                       const Matrix& a, const Matrix& b, size_t blockSize) {
    auto start = std::chrono::high_resolution_clock::now();
    Matrix result = multiplyFunc(a, b, blockSize);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << std::setw(15) << name << ": "
              << std::fixed << std::setprecision(6) << elapsed.count() << "s"
              << " (block size: " << blockSize << ")" << std::endl;
}

int main() {
    const size_t N = 500;
    const size_t minBlockSize = 1;
    const size_t maxBlockSize = N;

    Matrix a(N, N);
    Matrix b(N, N);
    a.fillRandom();
    b.fillRandom();

    std::cout << "Matrix size: " << N << "x" << N << std::endl;
    std::cout << "Testing sequential multiplication..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    Matrix seqResult = Matrix::multiplySequential(a, b);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> seqTime = end - start;

    std::cout << "Sequential: " << std::fixed << std::setprecision(6)
              << seqTime.count() << "s" << std::endl << std::endl;

    std::cout << "Testing parallel multiplication with different block sizes:" << std::endl;

    for (size_t blockSize = minBlockSize; blockSize <= maxBlockSize; blockSize *= 2) {
        std::cout << "Block size: " << blockSize << std::endl;

        #ifdef _WIN32
        testMultiplication("WinAPI", WinAPI::multiply, a, b, blockSize);
        #endif

        testMultiplication("std::thread", Thread::multiply, a, b, blockSize);
        testMultiplication("pthread", PThread::multiply, a, b, blockSize);

        std::cout << std::endl;
    }

    return 0;
}