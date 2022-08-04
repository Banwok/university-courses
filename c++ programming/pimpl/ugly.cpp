#include "ugly.h"
#include "bad.h"
#include <stdexcept>

class SplineImpl {
public:
    SplineImpl(const std::vector<double>& x, const std::vector<double>& y, double a, double b) {
        n_ = x.size();
        x_ = new double[n_];
        y_ = new double[n_];
        for (size_t i = 0; i < n_; ++i) {
            x_[i] = x[i];
            y_[i] = y[i];
        }
        a_ = a;
        b_ = b;
    }

    ~SplineImpl() {
        delete[] x_;
        delete[] y_;
    }

    double Interpolate(double x) {
        double* y2 = new double[n_];
        int flag = mySplineSnd(x_, y_, n_, a_, b_, y2);
        if (flag == -2) {
            throw std::runtime_error("x is not monitonic increasing");
        } else if (flag == -1) {
            throw std::runtime_error("Memory allocation error.");
        }
        double ans = 0;
        flag = mySplintCube(x_, y_, y2, n_, x, &ans);
        if (flag == -2) {
            throw std::runtime_error("x is not monitonic increasing");
        } else if (flag == -1) {
            throw std::runtime_error("Memory allocation error.");
        }
        delete[] y2;
        return ans;
    }

private:
    double* x_;
    double* y_;
    size_t n_;
    double a_;
    double b_;
};

Spline::Spline(const std::vector<double>& x, const std::vector<double>& y, double a, double b) {
    impl_ = std::make_shared<SplineImpl>(x, y, a, b);
}

double Spline::Interpolate(double x) {
    return impl_->Interpolate(x);
}