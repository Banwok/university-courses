/*
#include <cmath>
#include <vector>
#include <omp.h>
#include <iostream>

double a_p = 0.001, a_z = 0.0001, alfa_0 = 0.23, b_p = 0.01, b_z = 0.01, g_p = 0.001, q_p = 6,
       q_z = 5.5, Z0 = 0, Z1 = 1, P0 = 0, P1 = 1, k_z = 0.15, k_p = 0.05, Q0 = 5, Qs = 0.2,
       t_porog = 125000, a_r = 0.01, b_r = 0.01, R0 = 2, R1 = 1, q_r = 1.8, k_r = 0.1;

size_t t = 200000;
double t0 = 0, h = 0.05, x0 = 0.2, y0_ = 0.2, z0_ = 0.1;
double i_py = 0, i_end = 0.012;

double fx(double Z, double P, double t, double w_q) {
    return (-(a_z + g_p * P) * Z +
            b_z * (Z0 - (Z0 - Z1) / (1 + exp(-(Q0 + alfa_0 * Z + Qs * sin(w_q * t) - q_z) / k_z))));
}

double fy(double Z, double P, double t, double w_q) {
    return (-a_p * P +
            b_p * (P0 - (P0 - P1) / (1 + exp(-(Q0 + alfa_0 * Z + Qs * sin(w_q * t) - q_p) / k_p))));
}

double fz(double Z, double R, double t, double w_q) {
    return (-a_r * R +
            b_r * (R0 - (R0 - R1) / (1 + exp(-(Q0 + alfa_0 * Z + Qs * sin(w_q * t) - q_r) / k_r))));
}

void RKutt(std::vector<double>& a, std::vector<double>& b, double x0, double y0, double z0,
           size_t t, double h, double time0, double w_q, size_t& iter) {
    size_t size = static_cast<int>(t / h + 1);
    std::vector<double> x(size);
    std::vector<double> y(size);
    std::vector<double> z(size);
    double yt = 0;
    double xt = 0;
    double zt = 0;
    double t0 = time0;
    size_t i = 0;
    x[i] = x0;
    y[i] = y0;
    z[i] = z0;

    for (t0; t0 < t; t0 += h) {
        double k1_y = fy(x[i], y[i], t0, w_q);
        double k2_y = fy(x[i] + k1_y / 2.0, y[i] + k1_y / 2.0, t0, w_q);
        double k3_y = fy(x[i] + k2_y / 2.0, y[i] + k2_y / 2.0, t0, w_q);
        double k4_y = fy(x[i] + k3_y, y[i] + k3_y, t0, w_q);
        yt = y[i] + h * (k1_y + 2 * k2_y + 2 * k3_y + k4_y) / 6;
        y[i + 1] = yt;

        double k1_x = fx(x[i], y[i], t0, w_q);
        double k2_x = fx(x[i] + k1_x / 2.0, y[i] + k1_x / 2.0, t0, w_q);
        double k3_x = fx(x[i] + k2_x / 2.0, y[i] + k2_x / 2.0, t0, w_q);
        double k4_x = fx(x[i] + k3_x, y[i] + k3_x, t0, w_q);
        xt = x[i] + h * (k1_x + 2 * k2_x + 2 * k3_x + k4_x) / 6;
        x[i + 1] = xt;

        double k1_z = fz(x[i], z[i], t0, w_q);
        double k2_z = fz(x[i] + k1_z / 2.0, z[i] + k1_z / 2.0, t0, w_q);
        double k3_z = fz(x[i] + k2_z / 2.0, z[i] + k2_z / 2.0, t0, w_q);
        double k4_z = fz(x[i] + k3_z, z[i] + k3_z, t0, w_q);
        zt = z[i] + h * (k1_z + 2 * k2_z + 2 * k3_z + k4_z) / 6;
        z[i + 1] = zt;

        if ((t0 > t_porog) && ((x[i - 2] - x[i - 1]) * (x[i - 1] - x[i]) < 0) &&
            ((x[i - 2] - x[i - 1]) < 0)) {
            a[iter] = x[i - 1];
            b[iter] = w_q;
            ++iter;
        }
        ++i;
    }
}

void bifurcation(std::vector<double>& localM, std::vector<double>& w_s) {
    size_t iter = 0;
    std::vector<double> lmax(9000000);
    std::vector<double> w_SS(9000000);
    int ili = static_cast<int>(i_py * 100000);

    int Iend = static_cast<int>(i_end * 100000);


    for (int i = ili; i < Iend; i += 1) {
        double w_q = static_cast<double>(i) / 100000;
        RKutt(lmax, w_SS, x0, y0_, z0_, t, h, t0, w_q, iter);
    }
    lmax.erase(lmax.begin() + iter, lmax.end());
    w_SS.erase(w_SS.begin() + iter, w_SS.end());

    std::vector<double>(lmax).swap(lmax);
    std::vector<double>(w_SS).swap(w_SS);

    // lmax.erase(std::remove(lmax.begin(), lmax.end(), 0.0), lmax.end());
    // w_SS.erase(std::remove(w_SS.begin(), w_SS.end(), 0.0), w_SS.end());

    for (size_t k = 0; k < lmax.size(); ++k)
        localM.push_back(lmax[k]);

    for (size_t j = 0; j < w_SS.size(); ++j)
        w_s.push_back(w_SS[j]);
}

//--------------------------------------------------------------------------------------

void RK_wiki(std::vector<double>& x, std::vector<double>& y, std::vector<double>& z,
             std::vector<double>& time, double x0, double y0, double z0, size_t t, double h,
             double time0, double w_q) {
    double yt = 0;
    double xt = 0;
    double zt = 0;
    double t0 = time0;
    size_t i = 0;
    time[i] = t0;
    x[i] = x0;
    y[i] = y0;
    z[i] = z0;

    for (t0; t0 < t; t0 += h) {
        double k1_y = fy(x[i], y[i], t0, w_q);
        double k2_y = fy(x[i] + k1_y / 2.0, y[i] + k1_y / 2.0, t0, w_q);
        double k3_y = fy(x[i] + k2_y / 2.0, y[i] + k2_y / 2.0, t0, w_q);
        double k4_y = fy(x[i] + k3_y, y[i] + k3_y, t0, w_q);
        yt = y[i] + h * (k1_y + 2 * k2_y + 2 * k3_y + k4_y) / 6;
        y[i + 1] = yt;

        double k1_x = fx(x[i], y[i], t0, w_q);
        double k2_x = fx(x[i] + k1_x / 2.0, y[i] + k1_x / 2.0, t0, w_q);
        double k3_x = fx(x[i] + k2_x / 2.0, y[i] + k2_x / 2.0, t0, w_q);
        double k4_x = fx(x[i] + k3_x, y[i] + k3_x, t0, w_q);
        xt = x[i] + h * (k1_x + 2 * k2_x + 2 * k3_x + k4_x) / 6;
        x[i + 1] = xt;

        double k1_z = fz(x[i], z[i], t0, w_q);
        double k2_z = fz(x[i] + k1_z / 2.0, z[i] + k1_z / 2.0, t0, w_q);
        double k3_z = fz(x[i] + k2_z / 2.0, z[i] + k2_z / 2.0, t0, w_q);
        double k4_z = fz(x[i] + k3_z, z[i] + k3_z, t0, w_q);
        zt = z[i] + h * (k1_z + 2 * k2_z + 2 * k3_z + k4_z) / 6;
        z[i + 1] = zt;

        time[i + 1] = t0;
        ++i;
    }
}

void retuRelease(std::vector<double>& funX, std::vector<double>& funY, std::vector<double>& funZ,
                 std::vector<double>& funT, double w_q) {
    size_t size = static_cast<int>(t / h + 1);
    std::vector<double> x(size + 1);
    std::vector<double> y(size + 1);
    std::vector<double> z(size+ 1);
    std::vector<double> time(size+1);

    RK_wiki(x, y, z, time, x0, y0_, z0_, t, h, t0, w_q);

    std::cout<<"complited";

    for (size_t i = 0; i < 200; ++i) {
        std::cout<<z[i] << " ";
    }

    for (size_t i = 0; i < x.size(); ++i)
        funX.push_back(x[i]);

    for (size_t i = 0; i < y.size(); ++i)
        funY.push_back(y[i]);

    for (size_t i = 0; i < z.size(); ++i)
        funZ.push_back(z[i]);

    for (size_t i = 0; i < time.size(); ++i)
        funT.push_back(time[i]);
}

int main() {
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> time;
    double w_q = 0.01;

    retuRelease(x, y, z, time, w_q);
    return 0;
}
*/
