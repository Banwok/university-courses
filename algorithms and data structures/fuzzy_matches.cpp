/*
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<int> ZFunction(const std::string& str) {
    int left = 0;
    int right = 0;
    std::vector<int> zed(str.length());
    for (int iter = 1; iter < static_cast<int>(str.length()); ++iter) {
        if (iter <= right) {
            auto zi = iter - left;
            auto tmp = std::min(right - iter + 1, zed[zi]);
            zed[iter] = tmp;
        }
        while (iter + zed[iter] < static_cast<int>(str.length()) &&
               str[zed[iter]] == str[iter + zed[iter]]) {
            ++zed[iter];
        }
        if (iter + zed[iter] - 1 >= right) {
            left = iter;
            right = iter + zed[iter] - 1;
        } else {
            continue;
        }
    }
    zed[0] = str.length();
    return zed;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string f_str, s_str, rf_str, rs_str;
    std::vector<int> answer;
    std::cin >> f_str;
    std::cin >> s_str;
    rf_str = f_str;
    rs_str = s_str;
    std::reverse(rf_str.begin(), rf_str.end());
    std::reverse(rs_str.begin(), rs_str.end());

    std::vector<int> f_zed = ZFunction(f_str + "#" + s_str);
    std::vector<int> r_zed = ZFunction(rf_str + "#" + rs_str);
    int f_size = f_str.length();
    int s_size = s_str.length();
    f_zed = std::vector<int>(f_zed.begin() + f_size + 1, f_zed.end());
    r_zed = std::vector<int>(r_zed.begin() + rf_str.length() + 1, r_zed.end());
    std::reverse(r_zed.begin(), r_zed.end());

    for (int i = 0; i + f_size - 1 < s_size; ++i) {
        if (f_zed[i] + r_zed[i + f_size - 1] >= f_size - 1) {
            answer.push_back(i);
        }
    }

    std::cout << answer.size() << "\n";
    for (int& x : answer) {
        std::cout << x + 1 << " ";
    }

    return 0;
}
*/
