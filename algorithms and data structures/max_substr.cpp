/*
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int Periodicity(const std::string str) {
    int left = 0;
    int right = 0;
    int period = 1;
    std::vector<int> zed(str.length(), right);
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
    for (int i = 1; i < static_cast<int>(str.length()); ++i) {
        if (zed[i] >= i) {
            auto tmp = std::max(period, zed[i] / i + 1);
            period = tmp;
        }
    }
    return period;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string str;
    std::cin >> str;
    int ans = 1;
    for (int i = 0; i + 1 < static_cast<int>(str.length()); ++i) {
        ans = std::max(ans, Periodicity(str.substr(i, str.length())));
    }
    std::cout << ans;
    std::cout << "\n";
    return 0;
}
*/
