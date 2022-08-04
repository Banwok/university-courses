void LU_Decomposition(double* A, double* L, double* U, int n) {
    int r = 1800;
    if (n <= r) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                U[n * i + j] = A[n * i + j];
            }
        }
        for (int i = 0; i < n; ++i) {
            L[i * n + i] = 1;
            #pragma omp parallel for
            for (int k = i + 1; k < n; ++k) {
                double mu = U[k * n + i] / U[n * i + i];
                for (int j = i; j < n; ++j) {
                    U[k * n + j] -= mu * U[i * n + j];
                }
                L[k * n + i] = mu;
                L[i * n + k] = 0;
            }
        }
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                U[i * n + j] = 0;
            }
        }
    } else {
        int rr =r*r;
        int rnr = r * (n - r);
        int nrnr = (n - r) * (n - r);
        double* a11 = new double[rr];
        double* a12 = new double[rnr];
        double* a21 = new double[rnr];
        double* l11 = new double[rr];
        double* l21 = new double[rnr];
        double* u11 = new double[rr];
        double* u12 = new double[rnr];

        #pragma omp parallel for
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < r; ++j) {
                a11[i * r + j] = A[i * n + j];
            }
        }
        #pragma omp parallel for
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < n - r; ++j) {
                a12[i * (n - r) + j] = A[i * n + j + r];
            }
        }
        #pragma omp parallel for
        for (int i = 0; i < n - r; ++i) {
            for (int j = 0; j < r; ++j) {
                a21[i * r + j] = A[(i + r) * n + j];
            }
        }

        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < r; ++j) {
                u11[r * i + j] = a11[r * i + j];
            }
        }
        for (int i = 0; i < r; ++i) {
            l11[i * r + i] = 1;
            #pragma omp parallel for
            for (int k = i + 1; k < r; ++k) {
                double mu = u11[k * r + i] / u11[r * i + i];
                for (int j = i; j < r; ++j) {
                    u11[k * r + j] -= mu * u11[i * r + j];
                }
                l11[k * r + i] = mu;
                l11[i * r + k] = 0;
            }
        }
        for (int i = 1; i < r; ++i) {
            for (int j = 0; j < i; ++j) {
                u11[i * r + j] = 0;
            }
        }

        int k = n - r;
        #pragma omp parallel for
        for (int iter = 0; iter < k; ++iter) {
            for (int i = 0; i < r; ++i) {
                u12[k * i + iter] = a12[k * i + iter];
                for (int j = 0; j < i; ++j) {
                    u12[k * i + iter] -= l11[r * i + j] * u12[k * j + iter];
                }
                u12[k * i + iter] /= l11[r * i + i];
            }
        }
        
        #pragma omp parallel for
        for (int iter = 0; iter < k; ++iter) {
            for (int i = 0; i < r; ++i) {
                l21[r * iter + i] = a21[r * iter + i];
                for (int j = 0; j < i; ++j) {
                    l21[r * iter + i] -= u11[r * j + i] * l21[r * iter + j];
                }
                l21[r * iter + i] /= u11[r * i + i];
            }
        }

        delete[] a11;
        delete[] a12;
        delete[] a21;

        double* a22 = new double[nrnr];
        double* l22 = new double[nrnr];
        double* u22 = new double[nrnr];
        #pragma omp parallel for
        for (int i = 0; i < n - r; ++i) {
            for (int j = 0; j < n - r; ++j) {
                a22[i * (n - r) + j] = A[(i + r) * n + j + r];
            }
        }

        #pragma omp parallel for
        for (int i = 0; i < n - r; ++i) {
            for (int j = 0; j < n - r; ++j) {
                for (int k = 0; k < r; ++k) {
                    a22[i * (n - r) + j] -= l21[i * r + k] * u12[k * (n - r) + j];
                }
            }
        }
        LU_Decomposition(a22, l22, u22, n - r);
        #pragma omp parallel for
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < r; ++j) {
                L[i * n + j] = l11[i * r + j];
                U[i * n + j] = u11[i * r + j];
            }
        }
        #pragma omp parallel for
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < n - r; ++j) {
                L[i * n + j + r] = 0;
                U[i * n + j + r] = u12[i * (n - r) + j];
            }
        }
        #pragma omp parallel for
        for (int i = 0; i < n - r; ++i) {
            for (int j = 0; j < r; ++j) {
                L[(i + r) * n + j] = l21[i * r + j];
                U[(i + r) * n + j] = 0;
            }
        }
        #pragma omp parallel for
        for (int i = 0; i < n - r; ++i) {
            for (int j = 0; j < n - r; ++j) {
                L[(i + r) * n + j + r] = l22[i * (n - r) + j];
                U[(i + r) * n + j + r] = u22[i * (n - r) + j];
            }
        }

        delete[] a22;
        delete[] l21;
        delete[] l11;
        delete[] l22;
        delete[] u12;
        delete[] u11;
        delete[] u22;
    }
}
