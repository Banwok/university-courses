/*
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class SuffixArray {
public:
    SuffixArray(std::string str, int alphabet_size)
        : str_(str),
          alphabet_size_(alphabet_size),
          zero_permutation_(str.length()),
          counter_(alphabet_size, 0),
          zero_class_(str.length()),
          permutation_(str.length()),
          classe_(str.length()) {
    }

    void ZeroPhase() {
        for (int i = 0; i < str_.size(); ++i) {
            ++counter_[str_[i] - 'a'];
        }
        for (int i = 1; i < alphabet_size_; ++i) {
            counter_[i] += counter_[i - 1];
        }
        for (int i = 0; i < str_.size(); ++i) {
            zero_permutation_[--counter_[str_[i] - 'a']] = i;
        }
        zero_class_[zero_permutation_[0]] = 0;
        classes_ = 1;
        for (int i = 1; i < str_.length(); ++i) {
            if (str_[zero_permutation_[i]] != str_[zero_permutation_[i - 1]]) {
                ++classes_;
            }
            zero_class_[zero_permutation_[i]] = classes_ - 1;
        }
    }

    void Phases() {
        for (int power = 0; (1 << power) < str_.length(); ++power) {
            for (int i = 0; i < str_.length(); ++i) {
                permutation_[i] = zero_permutation_[i] - (1 << power);
                if (permutation_[i] < 0) {
                    permutation_[i] += static_cast<int>(str_.length());
                }
            }
            counter_.assign(classes_, 0);
            for (int i = 0; i < str_.length(); ++i) {
                ++counter_[zero_class_[permutation_[i]]];
            }
            for (int i = 1; i < classes_; ++i) {
                counter_[i] += counter_[i - 1];
            }
            for (int i = str_.length() - 1; i >= 0; --i) {
                zero_permutation_[--counter_[zero_class_[permutation_[i]]]] = permutation_[i];
            }
            classe_[zero_permutation_[0]] = 0;
            classes_ = 1;
            for (int i = 1; i < str_.length(); ++i) {
                int midf = (zero_permutation_[i] + (1 << power)) % str_.length();
                int mids = (zero_permutation_[i - 1] + (1 << power)) % str_.length();
                if (zero_class_[zero_permutation_[i]] != zero_class_[zero_permutation_[i - 1]] ||
                    zero_class_[midf] != zero_class_[mids]) {
                    ++classes_;
                }
                classe_[zero_permutation_[i]] = classes_ - 1;
            }
            zero_class_ = classe_;
        }
    }
    std::vector<int> GetPermutation() {
        std::vector<int> alt_permutation(permutation_.size());
        std::vector<int> cunt(classe_.size());
        for (int i = 0; i < classe_.size(); ++i) {
            ++cunt[classe_[i]];
        }
        for (int i = 1; i < classe_.size(); ++i) {
            cunt[i] += cunt[i - 1];
        }
        for (int i = 0; i < permutation_.size(); ++i) {
            alt_permutation[--cunt[classe_[i]]] = i;
        }
        std::vector<int> inverse_permutation(permutation_.size());
        for (int i = 0; i < permutation_.size(); ++i) {
            inverse_permutation[alt_permutation[i]] = i;
        }
        return alt_permutation;
    }

private:
    std::vector<int> zero_permutation_;
    std::vector<int> counter_;
    std::vector<int> zero_class_;
    std::vector<int> permutation_;
    std::vector<int> classe_;
    int classes_;
    std::string str_;
    int alphabet_size_;
};

int main() {
    std::string str;
    std::cin >> str;
    SuffixArray suffix_array(str, 26);
    suffix_array.ZeroPhase();
    suffix_array.Phases();
    std::vector<int> permutation = suffix_array.GetPermutation();
    std::string zipstr;
    for (int i = 0; i < permutation.size(); ++i) {
        int index = permutation[i];
        zipstr += str[(index + 2 * str.size() - 1) % str.size()];
    }
    std::cout << zipstr;
    return 0;
}*/
