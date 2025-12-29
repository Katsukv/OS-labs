#pragma once
#include <vector>
#include <iostream>
#include <chrono>

class Matrix {
public:
    Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, const std::vector<double>& data);

    size_t getRows() const;
    size_t getCols() const;
    double& at(size_t row, size_t col);
    const double& at(size_t row, size_t col) const;

    void fillRandom();
    void print() const;

    static Matrix multiplySequential(const Matrix& a, const Matrix& b);
    static Matrix multiplyParallel(const Matrix& a, const Matrix& b, size_t blockSize,
                                  void* (*threadFunc)(void*));

private:
    size_t m_rows;
    size_t m_cols;
    std::vector<double> m_data;
};

struct ThreadData {
    const Matrix* a;
    const Matrix* b;
    Matrix* result;
    size_t startRow;
    size_t endRow;
    size_t blockSize;
};