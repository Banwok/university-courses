#include <catch.hpp>
#include <matrix.h>

bool EqualMatrix(const Matrix& a, const Matrix& b) {
    if (a.Rows() != b.Rows()) {
        return false;
    }
    if (a.Columns() != b.Columns()) {
        return false;
    }
    for (size_t i = 0; i < a.Rows(); ++i) {
        for (size_t j = 0; j < a.Columns(); ++j) {
            if (a(i, j) != Approx(b(i, j))) {
                return false;
            }
        }
    }
    return true;
}

TEST_CASE("Constructors") {
    {
        Matrix a(3);
        REQUIRE(3u == a.Rows());
        REQUIRE(3u == a.Columns());
    }
    {
        Matrix a(3, 5);
        REQUIRE(3u == a.Rows());
        REQUIRE(5u == a.Columns());
    }
    {
        Matrix a({{1, 2}, {3, 4}, {5, 6}});
        REQUIRE(3u == a.Rows());
        REQUIRE(2u == a.Columns());
        REQUIRE(3 == a(1, 0));
    }
}

TEST_CASE("Constness") {
    {
        Matrix a({{1, 2}, {3, 4}});
        const Matrix& b = a;
        REQUIRE(2u == b.Rows());
        REQUIRE(2u == b.Columns());
        REQUIRE(2 == b(0, 1));
    }
    {
        const Matrix first = Identity(3);
        const Matrix second(3);
        REQUIRE(true == EqualMatrix(first, Transpose(first)));
        REQUIRE(true == EqualMatrix(second, first - first));
        REQUIRE(true == EqualMatrix(first, first * first));
    }
}

TEST_CASE("Operations") {
    Matrix a({{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}});
    Matrix b({{0.0, 1.0, 0.0}, {1.0, 1.0, 2.0}});
    Matrix c({{-1.0, -1.0}, {1.0, 1.0}, {1.0, -1.0}});

    REQUIRE(false == EqualMatrix(a, Transpose(a)));
    REQUIRE(true == EqualMatrix(Transpose(a), Matrix({{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}})));

    Matrix old_a = a;
    REQUIRE(true == EqualMatrix(a += b, Matrix({{1.0, 3.0, 3.0}, {5.0, 6.0, 8.0}})));
    REQUIRE(true == EqualMatrix(a -= b, old_a));
    REQUIRE(true == EqualMatrix(a -= a, Matrix(2, 3)));

    REQUIRE(true == EqualMatrix(b * c, Matrix({{1.0, 1.0}, {2.0, -2.0}})));
}
