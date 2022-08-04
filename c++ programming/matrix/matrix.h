#pragma once

#include <vector>

class Matrix {
protected:
    std::vector<std::vector<double>> matrix;
    size_t rows;
    size_t columns;

public:
    explicit Matrix(size_t row, size_t colum) {
        matrix.resize(row, std::vector<double>(colum));
        rows = row;
        columns = colum;
    }
    explicit Matrix(size_t n) : Matrix(n, n) {
    }
    explicit Matrix(std::vector<std::vector<double>> mat) {
        matrix = mat;
        rows = mat.size();
        columns = mat[0].size();
    }

    size_t Rows() const {
        return rows;
    }
    ssize_t Columns() const {
        return columns;
    }

    double& operator()(size_t in_row, size_t in_colum);
    const double& operator()(size_t in_row, size_t in_colum) const;
    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator-(const Matrix& lhs, const Matrix& rhs);
    friend Matrix Transpose(const Matrix& mat);

    Matrix& operator+=(const Matrix& rhs) {
        return *this = *this + rhs;
    }

    Matrix& operator*=(const Matrix& rhs) {
        return *this = *this * rhs;
    }

    Matrix& operator-=(const Matrix& rhs) {
        return *this = *this - rhs;
    }
};

double& Matrix::operator()(size_t in_row, size_t in_colum) {
    return matrix[in_row][in_colum];
}

const double& Matrix::operator()(size_t in_row, size_t in_colum) const {
    return matrix[in_row][in_colum];
}
Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    size_t r = lhs.Rows();
    size_t c = lhs.Columns();
    std::vector<std::vector<double>> tmp(r, std::vector<double>(c));
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            tmp[i][j] = lhs.matrix[i][j] + rhs.matrix[i][j];
        }
    }
    return Matrix(tmp);
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    size_t r = lhs.Rows();
    size_t c = rhs.Columns();
    std::vector<std::vector<double>> tmp(r, std::vector<double>(c));
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            tmp[i][j] = 0;
            for (size_t k = 0; k < lhs.Columns(); ++k) {
                tmp[i][j] += lhs.matrix[i][k] * rhs.matrix[k][j];
            }
        }
    }
    return Matrix(tmp);
}

Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    size_t r = lhs.Rows();
    size_t c = lhs.Columns();
    std::vector<std::vector<double>> tmp(r, std::vector<double>(c));
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            tmp[i][j] = lhs.matrix[i][j] - rhs.matrix[i][j];
        }
    }
    return Matrix(tmp);
}

Matrix Transpose(const Matrix& mat) {
    size_t r = mat.Rows();
    size_t c = mat.Columns();
    std::vector<std::vector<double>> tmp(c, std::vector<double>(r));
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            tmp[j][i] = mat.matrix[i][j];
        }
    }
    return Matrix(tmp);
}

Matrix Identity(size_t n) {
    std::vector<std::vector<double>> tmp(n, std::vector<double>(n));
    size_t j = 0;
    for (size_t i = 0; i < n; ++i) {
        tmp[i][j] = 1;
        ++j;
    }
    return Matrix(tmp);
}