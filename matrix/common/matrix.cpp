#include "matrix.h"
#include <random>
#include <iomanip>

Matrix::Matrix(size_t rows, size_t cols)
    : m_rows(rows), m_cols(cols), m_data(rows * cols, 0.0) {}

Matrix::Matrix(size_t rows, size_t cols, const std::vector<double>& data)
    : m_rows(rows), m_cols(cols), m_data(data) {
    if (data.size() != rows * cols) {
        throw std::invalid_argument("Invalid data size");
    }
}

size_t Matrix::getRows() const { return m_rows; }
size_t Matrix::getCols() const { return m_cols; }

double& Matrix::at(size_t row, size_t col) {
    return m_data[row * m_cols + col];
}

const double& Matrix::at(size_t row, size_t col) const {
    return m_data[row * m_cols + col];
}

void Matrix::fillRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10.0);

    for (auto& val : m_data) {
        val = dis(gen);
    }
}

void Matrix::print() const {
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            std::cout << std::setw(8) << std::setprecision(3) << at(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::multiplySequential(const Matrix& a, const Matrix& b) {
    if (a.getCols() != b.getRows()) {
        throw std::invalid_argument("Matrix dimensions mismatch");
    }

    Matrix result(a.getRows(), b.getCols());

    for (size_t i = 0; i < a.getRows(); ++i) {
        for (size_t j = 0; j < b.getCols(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < a.getCols(); ++k) {
                sum += a.at(i, k) * b.at(k, j);
            }
            result.at(i, j) = sum;
        }
    }

    return result;
}

Matrix Matrix::multiplyParallel(const Matrix& a, const Matrix& b, size_t blockSize,
                               void* (*threadFunc)(void*)) {
    if (a.getCols() != b.getRows()) {
        throw std::invalid_argument("Matrix dimensions mismatch");
    }

    Matrix result(a.getRows(), b.getCols());

    size_t numBlocks = (a.getRows() + blockSize - 1) / blockSize;
    std::vector<pthread_t> threads(numBlocks);
    std::vector<ThreadData> threadData(numBlocks);

    for (size_t i = 0; i < numBlocks; ++i) {
        size_t startRow = i * blockSize;
        size_t endRow = std::min(startRow + blockSize, a.getRows());

        threadData[i] = {&a, &b, &result, startRow, endRow, blockSize};

        if (pthread_create(&threads[i], nullptr, threadFunc, &threadData[i]) != 0) {
            throw std::runtime_error("Failed to create thread");
        }
    }

    for (size_t i = 0; i < numBlocks; ++i) {
        pthread_join(threads[i], nullptr);
    }

    return result;
}