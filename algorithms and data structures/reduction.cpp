/*
#include <cmath>
#include <vector>

class heat_task {
public:
    double T;
    double L;
    int n;
    int m;
    double initial_condition(double x) {
        return sin(x * M_PI) + sin(3 * M_PI * x);
    }
    double left_condition(double t) {
        return 0;
    }
    double right_condition(double t) {
        return 0;
    }
    double f(double x, double t) {
        return 0;
    }
};

void heat_equation_crank_nicolson(heat_task task, double *v) {
    std::vector<double> g(task.n + 1);
    double h = task.L / task.n;
    double tao = task.T / task.m;

#pragma omp parallel for
    for (int i = 0; i <= task.n; ++i) {
        g[i] = task.initial_condition(i * h);
    }

    auto logar_n = std::log2(task.n);
    double ac_val = (tao / (2 * h * h));
    double b_val = -(1 + tao / (h * h));
    double right_one = (tao / (h * h) - 1);
    double right_two = (tao / (2 * h * h));

    {
        std::vector<double> a(logar_n + 1);
        std::vector<double> b(logar_n + 1);
        std::vector<double> c(logar_n + 1);
        std::vector<double> right(task.n + 1);

        for (int j = 1; j <= task.m; ++j) {
#pragma omp parallel for
            for (int i = 2; i <= task.n; ++i) {
                right[i - 1] = right_one * g[(i - 1)] -
                               right_two * (g[(i - 1) - 1] + g[(i - 1) + 1]) -
                               tao * task.f((i - 1) * h, ((j - 1) + 0.5) * tao);
            }
            right[1] -= task.left_condition((j)*tao) * (tao / (h * h * 2));
            right[task.n - 1] -= task.right_condition((j)*tao) * (tao / (h * h * 2));

            a[0] = ac_val;
            b[0] = b_val;
            c[0] = ac_val;
            right[0] = 0;
            right[task.n] = 0;
            g[0] = 0;
            g[task.n] = 0;

            int start = 2;
            int size_n = task.n;
            int step = 1;
            for (int jiter = 0; jiter < logar_n; ++jiter) {
                double alpha = -a[jiter] / b[jiter];
                double beta = -c[jiter] / b[jiter];
                a[jiter + 1] = alpha * a[jiter];
                b[jiter + 1] = b[jiter] + alpha * a[jiter] + beta * c[jiter];
                c[jiter + 1] = beta * c[jiter];

                size_n = (size_n - 1) / 2;
#pragma omp parallel for
                for (int i = 0; i < size_n; ++i) {
                    int k = start * (i + 1);
                    right[k] = alpha * right[k - step] + right[k] + beta * right[k + step];
                }
                start = 2 * start;
                step = 2 * step;
            }

            start = task.n / 2;
            size_n = 1;
            for (int jiter = logar_n - 1; jiter >= 0; --jiter) {
                double alpha = -a[jiter] / b[jiter];
                double beta = -c[jiter] / b[jiter];
#pragma omp parallel for
                for (int i = 0; i < size_n; ++i) {
                    int k = start * (2 * i + 1);
                    g[k] = right[k] / b[jiter] + alpha * g[k - start] + beta * g[k + start];
                }
                start = start / 2;
                size_n = size_n * 2;
            }

            g[0] = task.left_condition(j * tao);
            g[task.n] = task.right_condition(j * tao);
        }
#pragma omp parallel for
        for (int i = 0; i <= task.n; ++i) {
            v[i] = g[i];
        }
    }
}*/
