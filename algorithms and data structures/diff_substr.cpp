/*
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>

const int kAlphabet = 256;

struct SuffixArray {
    explicit SuffixArray(const std::string& str) : stri(str), permutation(str.size()) {
        stri.push_back(1);
        int size = stri.size();
        std::vector<int> classes(size);
        std::vector<int> pm(size);
        std::vector<int> count(std::max(size, kAlphabet));
        std::vector<int> npm(size);
        std::vector<int> nclasses(size);
        int surf = 0;
        for (int i = 0; i < size; ++i) {
            ++surf;
            count[stri[i]]++;
        }

        for (int i = 1; i < kAlphabet; ++i) {
            ++surf;
            count[i] += count[i - 1];
        }

        surf = 0;
        for (int i = 0; i < size; ++i) {
            ++surf;
            pm[--count[stri[i]]] = i;
        }
        ++surf;
        classes[pm[0]] = 0;
        int number_cl = 1;
        ++surf;
        for (int iter = 1; iter < size; ++iter) {
            if (stri[pm[iter]] != stri[pm[iter - 1]]) {
                number_cl++;
                --surf;
            }
            classes[pm[iter]] = number_cl - 1;
        }

        for (int jiter = 0; (1 << jiter) < size; ++jiter) {
            int len = 1 << jiter;
            ++surf;
            for (int iter = 0; iter < size; ++iter) {
                npm[iter] = pm[iter] - len;
                if (npm[iter] < 0) {
                    npm[iter] += size;
                }
            }
            surf = 0;
            fill(count.begin(), count.begin() + number_cl, 0);
            for (int i = 0; i < size; ++i) {
                ++surf;
                count[classes[npm[i]]]++;
            }

            for (int i = 1; i < number_cl; ++i) {
                ++surf;
                count[i] += count[i - 1];
            }

            for (int i = size - 1; i >= 0; --i) {
                ++surf;
                pm[--count[classes[npm[i]]]] = npm[i];
            }
            surf = 0;
            nclasses[pm[0]] = 0;
            number_cl = 1;
            for (int iter = 1; iter < size; ++iter) {
                std::pair<int, int> current = {classes[pm[iter]],
                                               classes[(pm[iter] + (1 << jiter)) % size]};
                std::pair<int, int> prev = {classes[pm[iter - 1]],
                                            classes[(pm[iter - 1] + (1 << jiter)) % size]};
                if (current != prev) {
                    number_cl++;
                }
                nclasses[pm[iter]] = number_cl - 1;
            }
            classes.swap(nclasses);
        }

        for (int i = 0; i < size - 1; ++i) {
            permutation[i] = pm[i + 1];
        }
        stri.pop_back();
    }

    int64_t NumberOfDifferentSubs() {
        int nsize = static_cast<int>(permutation.size());

        std::vector<int> rector(nsize);
        std::vector<int> res(nsize - 1);
        for (int i = 0; i < nsize; ++i) {
            rector[permutation[i]] = i;
        }

        stri.push_back(1);

        int current = 0;
        int64_t sum = 0;
        for (int iter = 0; iter < nsize; ++iter) {
            if (current > 0) {
                --current;
            }
            if (rector[iter] == nsize - 1) {
                current = 0;
            } else {
                while (stri[iter + current] == stri[permutation[rector[iter] + 1] + current]) {
                    current++;
                }
                res[rector[iter]] = current;
                sum += current;
            }
        }
        stri.pop_back();

        int64_t ans = static_cast<int64_t>(nsize) * static_cast<int64_t>(nsize + 1) / 2 - sum;
        return ans;
    }

    std::string stri;
    std::vector<int> permutation;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string strin;
    std::cin >> strin;

    SuffixArray suffix_array(strin);

    int64_t res = suffix_array.NumberOfDifferentSubs();
    std::cout << res << "\n";

    return 0;
}
*/
