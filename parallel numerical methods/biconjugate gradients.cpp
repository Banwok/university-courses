double ScalarProduct(double *a, double *b, int n) {
    double res = 0.0;
    #pragma omp parallel for reduction(+:res)
    for (int i = 0; i < n; ++i) {
        res += a[i] * b[i];
}
    return res;
}

void MultMatrVect(CRSMatrix &A, double *b, double *mult) {
    #pragma omp parallel for
    for (int i = 0; i < A.n; ++i) {
        mult[i] = 0.0;
        for (int j = A.rowPtr[i]; j < A.rowPtr[i + 1]; ++j) {
            mult[i] += A.val[j] * b[A.colIndex[j]];
}
    }
}

void TranspMatr(CRSMatrix &A, CRSMatrix &A_transp) {
    A_transp.n = A.m;
    A_transp.m = A.n;
    A.nz = A_transp.nz;
    std::vector<std::vector<int>> index(A_transp.n);
    std::vector<std::vector<double>> value(A_transp.n);
    std::vector<int> size(A_transp.n);

    for (int i = 0; i < A.n; ++i) {
        for (int j = A.rowPtr[i]; j < A.rowPtr[i + 1]; ++j) {
            index[A.colIndex[j]].push_back(i);
            value[A.colIndex[j]].push_back(A.val[j]);
            size[A.colIndex[j]] += 1;
        }
    }
    A_transp.rowPtr.push_back(0);
    for (int i = 0; i < A_transp.n; ++i) {
        A_transp.rowPtr.push_back(A_transp.rowPtr[i] + size[i]);
        for (int j = 0; j < size[i]; ++j) {
            A_transp.val.push_back(value[i][j]);
            A_transp.colIndex.push_back(index[i][j]);
        }
    }
}

void SLE_Solver_CRS_BICG(CRSMatrix &A, double *b, double eps, int max_iter, double *x, int &count) {
    count = 0;
    int n_size = A.n;
    CRSMatrix a_tr;
    TranspMatr(A, a_tr);

    double *r = new double[n_size];
    double *r_ = new double[n_size];
    double *r_next = new double[n_size];
    double *r_next_ = new double[n_size];
    double *p = new double[n_size];
    double *p_ = new double[n_size];
    double *Ap = new double[n_size];
    double *Ap_ = new double[n_size];
    double alpha = 0.0;
    double betta = 0.0;
    double norm = sqrt(ScalarProduct(b, b, n_size));

    #pragma omp parallel for
    for (int i = 0; i < n_size; ++i) {
        x[i] = 1.0;
    }

    MultMatrVect(A, x, Ap);

    #pragma omp parallel for
    for (int i = 0; i < n_size; ++i) {
        r[i] = b[i] - Ap[i];
        r_[i] = r[i];
        p[i] = r[i];
        p_[i] = r_[i];
    }

     for (;count < max_iter; ++count){
        if (sqrt(ScalarProduct(r, r, n_size)) / norm < eps) {
            break;
}

        MultMatrVect(A, p, Ap);
        MultMatrVect(a_tr, p_, Ap_);
        double scalar_tmp = ScalarProduct(r, r_, n_size);
        alpha =  scalar_tmp / ScalarProduct(Ap, p_, n_size);

        #pragma omp parallel for
        for (int i = 0; i < n_size; ++i) {
            x[i] += alpha * p[i];
            r_next[i] = r[i] - alpha * Ap[i];
            r_next_[i] = r_[i] - alpha * Ap_[i];
        }

        betta = ScalarProduct(r_next, r_next_, n_size) / scalar_tmp;

        #pragma omp parallel for
        for (int i = 0; i < n_size; ++i) {
            p[i] = r_next[i] + betta * p[i];
            p_[i] = r_next_[i] + betta * p_[i];
            r[i] = r_next[i];
            r_[i] = r_next_[i];
        }
    }

    delete[] r;
    delete[] r_;
    delete[] r_next;
    delete[] r_next_;
    delete[] p;
    delete[] p_;
    delete[] Ap;
    delete[] Ap_;
}
