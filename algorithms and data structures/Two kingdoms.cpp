#include <iostream>
#include <vector>
#include "omp.h"

#include <fstream>
#include <algorithm>
#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace std::chrono;

class heat_task {
public:
    double X = 2;
    double Y = 2;
    int n = 256;
    int m = 256;
    double left_condition(double y) { return 1.0 + y * y; }
    double right_condition(double y) { return 4.0 + y * y; }
    double bottom_condition(double x) { return 4.0 + x * x; }
    double top_condition(double x) { return 9.0 + x * x; }
    double f(double x, double y) { return 4.0; }
};

//void heat_dirichlet_sor(heat_task task, double* v) {
//    double h = task.X / task.n;
//    double k = task.Y / task.m;
//
//    double h_ = 1 / (h * h);
//    double k_ = 1 / (k * k);
//    double D = 2 * (h_ + k_);
//
//    double omega = 2 / (1 + sin(M_PI * min(h, k) / 2));
//
//
//#pragma omp parallel for
//    for (int i = 0; i <= task.n; ++i) {
//        v[i * (task.m + 1)] = task.bottom_condition(i * h);
//        v[i * (task.m + 1) + task.m] = task.top_condition(i * h);
//    }
//#pragma omp parallel for
//    for (int j = 0; j <= task.m; ++j) {
//        v[j] = task.left_condition(j * k);
//        v[task.n * (task.m + 1) + j] = task.right_condition(j * k);
//    }
//
//#pragma omp parallel for
//    for (int i = 1; i < task.n; ++i) {
//        for (int j = 1; j < task.m; j++) {
//            v[i * (task.m + 1) + j] = 0.;
//        }
//    }
//
//    vector<vector<double>> f(task.n + 1, vector<double>(task.m + 1, 0.0));
//#pragma omp parallel for
//    for (int i = 0; i <= task.n; ++i) {
//        for (int j = 0; j <= task.m; ++j) {
//            f[i][j] = -task.f(i * h, j * k);
//        }
//    }
//
//    double prev;
//    int top_index, bot_index, left_index, right_index, curr_index;
//
//    int max_iter = 1.5 / min(h, k) / M_PI * log(1 / 1e-7);
//    vector<double> err((task.n + 1) * (task.m + 1), 0.);
//    for (int iteration = 0; iteration < max_iter; ++iteration) {
//
//        //cout << "task.n + task.m - 3 = " << task.n + task.m - 3 << endl;
//
//        //int sizebl = 4;
//
//        //cout << "task.n " << task.n << endl;
//
//        //for (int bl = 0; bl < 1; bl += sizebl) {//task.n + task.m - 3
//        //{
//            //int start_bl = min(1 + bl, task.n - 1);
//            //int finish_bl = max(1, bl - task.m + 3);
//
//            //cout << "------------------------------------------------------------------------ " << endl;
//            //cout << "bl = " << bl << ", start_bl = " << start_bl << ", finish_bl = " << finish_bl << endl;
//
//            //for (int k = 0; k < 1; ++k) {//sizebl + sizebl  task.n + task.m
//
//
//                //int start = min(1 + k, task.n - 1);
//                //int finish = max(1, k - task.m + 3);
//
//                //cout << "k = " << k << ", start = " << start << ", finish = " << finish << endl;
//                //cout << "1 + k = " << 1 + k << ", task.n - 1 = " << task.n - 1 << endl;
//                //cout << "1 " << 1 << ", k - task.m + 3 = " << k - task.m + 3 << endl;
//
//                for (int i = 1; i >= task.n; ++i) {
//                    //#pragma omp parallel for
//                    for (int j = 1; i >= task.m; ++j) {
//
//
//                        curr_index = i * (task.m + 1) + j;
//
//                        cout << i << " " << j << " " << "index " << curr_index << endl;
//                        prev = v[curr_index];
//
//                        top_index = curr_index + 1;
//                        bot_index = curr_index - 1;
//                        left_index = curr_index - (task.m + 1);
//                        right_index = curr_index + (task.m + 1);
//
//                        v[curr_index] = (omega * (h_ * (v[left_index] + v[right_index]) + k_ * (v[top_index] + v[bot_index]) + f[i][j]) + (1 - omega) * D * prev) / D;
//                        err[curr_index] = abs(v[curr_index] - prev);
//
//
//                        cout << "left_index " << left_index << " right_index " << right_index << ", " << v[left_index] << " " << v[right_index] << endl;
//                        cout << "top_index " << top_index << " bot_index " << bot_index << ", " << v[top_index] << " " << v[bot_index] << endl;
//                        //cout << "v[curr_index] " << v[curr_index] << ", prev " << prev << ", ddd " << err[curr_index] << endl;
//                        //cout << endl;
//                    }cout << endl;
//                }
//           // }
//            auto maxerr = *max_element(err.begin(), err.end());
//            if (maxerr < 1e-9) {
//                cout << "break iteration = " << iteration << endl;
//                break;
//            }
//
//        //}
//    }
//}
void heat_dirichlet_sor(heat_task task, double* v) {
    double h = task.X / task.n;
    double k = task.Y / task.m;

    double h_ = 1 / (h * h);
    double k_ = 1 / (k * k);
    double D = 2 * (h_ + k_);

    double omega = 2 / (1 + sin(M_PI * min(h, k) / 2));


#pragma omp parallel for
    for (int i = 0; i <= task.n; ++i) {
        v[i * (task.m + 1)] = task.bottom_condition(i * h);
        v[i * (task.m + 1) + task.m] = task.top_condition(i * h);
    }
#pragma omp parallel for
    for (int j = 0; j <= task.m; ++j) {
        v[j] = task.left_condition(j * k);
        v[task.n * (task.m + 1) + j] = task.right_condition(j * k);
    }

#pragma omp parallel for
    for (int i = 1; i < task.n; ++i) {
        for (int j = 1; j < task.m; j++) {
            v[i * (task.m + 1) + j] = 0.;
        }
    }

    vector<vector<double>> f(task.n + 1, vector<double>(task.m + 1, 0.0));
#pragma omp parallel for
    for (int i = 0; i <= task.n; ++i) {
        for (int j = 0; j <= task.m; ++j) {
            f[i][j] = -task.f(i * h, j * k);
        }
    }

    double prev;
    int top_index, bot_index, left_index, right_index, curr_index;

    int max_iter = 1.5 / min(h, k) / M_PI * log(1 / 1e-7);
    vector<double> err((task.n + 1) * (task.m + 1), 0.);

    vector <vector<int>> points;

    int sizebl = 4;

    int num_row = (task.n - 1) / sizebl;
    int num_col = (task.m - 1) / sizebl;
    int num = min(num_row, num_col);
    //cout << num << " :" << num_row << " " << num_col << endl << endl;

    for (int iter = 0; iter < num; ++iter) {
        for (int i = iter * sizebl + 1, j = 1; i >= 1, j <= iter * sizebl + 1; i -= sizebl, j += sizebl) {
            points.push_back({ i,j });
        }
    }

    for (int iter = 1; iter <= num - 1; iter += 1) {
        for (int i = (num - 1) * sizebl + 1, j = iter * sizebl + 1; i > iter * sizebl, j < num * sizebl; i -= sizebl, j += sizebl) {
            points.push_back({ i,j });
        }
    }

    //for (int i = 0; i < points.size(); i++) {
    //	for (int j = 0; j < points[0].size(); j++) {
    //		//cout << points[i][j] << " ";
    //	}//cout << endl;
    //}

    for (int iteration = 0; iteration < max_iter; ++iteration) {

        for (int ind = 0; ind < points.size(); ++ind) {

            for (int i = points[ind][0]; i < points[ind][0] + sizebl; ++i) {

                for (int j = points[ind][1]; j < points[ind][1] + sizebl; ++j) {

                    curr_index = i * (task.m + 1) + j;

                    //cout << i << " " << j << " " << "index " << curr_index << endl;
                    prev = v[curr_index];

                    top_index = curr_index + 1;
                    bot_index = curr_index - 1;
                    left_index = curr_index - (task.m + 1);
                    right_index = curr_index + (task.m + 1);

                    v[curr_index] = (omega * (h_ * (v[left_index] + v[right_index]) + k_ * (v[top_index] + v[bot_index]) + f[i][j]) + (1 - omega) * D * prev) / D;
                    err[curr_index] = abs(v[curr_index] - prev);

                    //cout << "left_index " << left_index << " right_index " << right_index << ", " << v[left_index] << " " << v[right_index] << endl;
                    //cout << "top_index " << top_index << " bot_index " << bot_index << ", " << v[top_index] << " " << v[bot_index] << endl;
                    //cout << "v[curr_index] " << v[curr_index] << ", prev " << prev << ", ddd " << err[curr_index] << endl;
                    //cout << endl;
                }
                //cout << endl;
            }

        }

        int size_points = points.size();
        //int left_down = num * sizebl + 2 + task.m;
        //int left_up = (points[size_points - 1][0] + sizebl - 1) * (task.m + 1) + points[size_points - 1][1] + sizebl - 1 + 1;

        int left_down = points[size_points - 1][1] + sizebl;
        int left_up = points[size_points - 1][0] + sizebl ;

        //cout << "left_down " << left_down << " left_up " << left_up << endl;
        for (int i = 1; i < left_up; ++i) {

            for (int j = left_down; j < task.n; ++j) {
                curr_index = i * (task.m + 1) + j;
                //cout << i << " " << j << " " << "index " << curr_index << endl;


                prev = v[curr_index];

                top_index = curr_index + 1;
                bot_index = curr_index - 1;
                left_index = curr_index - (task.m + 1);
                right_index = curr_index + (task.m + 1);

                v[curr_index] = (omega * (h_ * (v[left_index] + v[right_index]) + k_ * (v[top_index] + v[bot_index]) + f[i][j]) + (1 - omega) * D * prev) / D;
                err[curr_index] = abs(v[curr_index] - prev);
            }
        }

        //cout << "left_down !" << left_down << " left_up " << left_up << endl;
        for (int i = left_up; i < task.m; ++i) {

            for (int j = 1; j < task.n; ++j) {
                curr_index = i * (task.m + 1) + j;
                //cout << i << " " << j << " " << "index " << curr_index << endl;

                prev = v[curr_index];

                top_index = curr_index + 1;
                bot_index = curr_index - 1;
                left_index = curr_index - (task.m + 1);
                right_index = curr_index + (task.m + 1);

                v[curr_index] = (omega * (h_ * (v[left_index] + v[right_index]) + k_ * (v[top_index] + v[bot_index]) + f[i][j]) + (1 - omega) * D * prev) / D;
                err[curr_index] = abs(v[curr_index] - prev);
            }
        }

        auto maxerr = *max_element(err.begin(), err.end());
        if (maxerr < 1e-9) {
            //cout << "break iteration = " << iteration << endl;
            break;
        }
    }
}


void PrintArraymy(int size, double* a) {
    std::cout << "( ";
    for (int i = 0; i < size; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << ")" << std::endl;
}

int main(int argc, char* argv[]) {
    heat_task t;
    double* v = new double[(t.n + 1) * (t.m + 1)];

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    heat_dirichlet_sor(t, v);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double, std::milli> time_span = t2 - t1;

    //std::cout << "Duration time?: " << time_span.count() << " milliseconds.";

    PrintArraymy((t.n + 1) * (t.m + 1), v);

    delete[] v;


    //for (double h = 0; h < 1; h += 0.0005) {
    //    int max_iter = 1.5 / h / M_PI * log(1 / 1e-7);
    //    cout << h << " " << max_iter << endl;
    //}


    return 0;
}