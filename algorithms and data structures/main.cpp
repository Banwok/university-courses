/*
#include <iostream>
#include <vector>

std::vector<int> mergeTwo(std::vector<int>& left, std::vector<int>& right, size_t m_size) {
    std::vector<int> res;
    size_t l_in = 0;
    size_t r_in = 0;
    while (l_in + r_in < left.size() + right.size()) {
        if (l_in != left.size() && (r_in == right.size() || left[l_in] < right[r_in])) {
            res.push_back(left[l_in]);
            ++l_in;
        } else {
            res.push_back(right[r_in]);
            ++r_in;
        }
    }
    return res;
}

std::vector<int> mergeVec(std::vector<std::vector<int>>& vec, size_t m_size) {
    std::vector<std::vector<int>> tmp_vec;
    while (vec.size() != 1) {
        tmp_vec.clear();
        for (size_t iter = 0; iter < vec.size(); iter += 2) {
            if (iter != vec.size() - 1) {
                tmp_vec.push_back(mergeTwo(vec[iter], vec[iter + 1], m_size));
            } else {
                tmp_vec.push_back(vec[iter]);
            }
        }
        vec = tmp_vec;
    }
    return vec[0];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t n_size, m_size;
    std::cin >> n_size >> m_size;
    std::vector<std::vector<int>> vec(n_size, std::vector<int>(m_size));
    for (size_t i = 0; i < n_size; ++i) {
        for (size_t j = 0; j < m_size; ++j) {
            std::cin >> vec[i][j];
        }
    }
    std::cout<<vec.size()<<std::endl;
    std::vector<int> output = mergeVec(vec, m_size);
    for (int& it : output) {
        std::cout << it << " ";
    }

    return 0;
}
*/
