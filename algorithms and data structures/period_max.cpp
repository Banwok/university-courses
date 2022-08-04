/*
#include <iostream>
#include <vector>
#include <string>

signed main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string strin, tstr;
    std::cin >> tstr;
    int64_t n = static_cast<int64_t>(tstr.size());
    std::vector<int64_t> hector(n + 1);
    std::vector<int64_t> pector(n + 1);
    std::vector<int64_t> hec(n + 1);
    std::vector<int64_t> pec(n + 1);

    int64_t m = 1e9 + 7;
    int64_t b = 27;
    hector[0] = 0;
    pector[0] = 1;
    hec[0] = 0;
    pec[0] = 1;

    auto get_one = [&](int64_t l, int64_t r) -> int64_t {
        return (hector[r + 1] - (hector[l] * pector[r - l + 1]) % m + m) % m;
    };

    auto get_two = [&](int64_t l, int64_t r) -> int64_t {
        return (hec[r + 1] - (hec[l] * pec[r - l + 1]) % m + m) % m;
    };

    for (int64_t iter = 0; iter < n; ++iter) {
        hector[iter + 1] = (hector[iter] * b + (tstr[iter] - 'a' + 1)) % m;
        pector[iter + 1] = (pector[iter] * b) % m;
    }

    for (int64_t iter = 1; iter <= static_cast<int64_t>(tstr.size()); ++iter) {
        strin += tstr[iter - 1];
        hec[iter] = (hec[iter - 1] * b + (strin[iter - 1] - 'a' + 1)) % m;
        pec[iter] = (pec[iter - 1] * b) % m;
        int64_t j = 0;
        int64_t ep = static_cast<int64_t>(strin.size());
        for (int64_t e = 0; e <= n - ep; e += iter) {
            if (get_one(e, e + ep - 1) == get_two(0, ep - 1)) {
                ++j;
            }
        }
        if (n % iter == 0 && static_cast<int64_t>(tstr.size()) / iter == j) {
            std::cout << static_cast<int64_t>(tstr.size()) / iter;
            return 0;
        }
    }
    return 0;
}
*/
