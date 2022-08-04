#pragma once

#include <array>
#include <cmath>
#include <vector.h>

struct CameraOptions {
    int screen_width;
    int screen_height;
    double fov;
    Vector look_from;
    Vector look_to;

    CameraOptions(int width, int height, double fov = M_PI / 2,
                  std::array<double, 3> look_from = {0.0, 0.0, 0.0},
                  std::array<double, 3> look_to = {0.0, 0.0, -1.0})
        : screen_width(width),
          screen_height(height),
          fov(fov),
          look_from(look_from),
          look_to(look_to)

    {
    }
};

class RotationMatrix {
public:
    RotationMatrix(const Vector& from, const Vector& to) {
        Vector direction = to - from;
        columns_.reserve(3);
        direction.Normalize();
        Vector forward = -1 * direction;
        Vector right{0, 1, 0};
        right = CrossProduct(right, forward);
        right = (Length(right) < 1e-9) ? Vector{1, 0, 0} : right;
        right.Normalize();

        Vector up = CrossProduct(forward, right);
        up.Normalize();
        for (size_t i = 0; i < 3; ++i) {
            columns_.push_back(Vector({right[i], up[i], forward[i]}));
        }
    }

private:
    friend Vector operator*(const RotationMatrix&, const Vector&);
    std::vector<Vector> columns_;
};

Vector operator*(const RotationMatrix& matrix, const Vector& vector) {
    std::array<double, 3> data;
    for (size_t i = 0; i < 3; ++i) {
        data[i] = DotProduct(matrix.columns_[i], vector);
    }
    auto res = Vector(data);
    return res;
}

class Camera {
public:
    Camera(const CameraOptions* options) : matrix_{options->look_from, options->look_to} {
        options_ = options;
        height_ = 2 * tan(options->fov / 2);
        width_ = height_ / options->screen_height * options->screen_width;
    }

    Vector GetDirection(int i, int j) {
        double x = (2 * (i + 0.5) / options_->screen_width - 1) * width_ / 2;
        double y = (2 * (-j - 0.5) / options_->screen_height + 1) * height_ / 2;
        Vector direction{x, y, -1};
        direction = matrix_ * direction;
        direction.Normalize();
        return direction;
    }

private:
    const CameraOptions* options_;
    RotationMatrix matrix_;
    double width_, height_;
};
