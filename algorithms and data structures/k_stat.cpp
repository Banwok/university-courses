/*
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

class Heap {
public:
    std::vector<int> indicators_;
    int len_;

    explicit Heap(int len) {
        len_ = len;
        indicators_.assign(len_, 0);
    }

    int Sum(int index) {
        int result = 0;
        for (; index >= 0; index = (index & (index + 1)) - 1) {
            result += indicators_[index];
        }
        return result;
    }

    bool Empty() {
        if (indicators_.empty()) {
            return true;
        }
        return false;
    }

    int GetStat(int kth) {
        int kth_check = 0;
        if (Empty()) {
            return 0;
        }
        for (auto index = len_ - 1; index >= 0; index = (index & (index + 1)) - 1) {
            kth_check += indicators_[index];
        }
        int lower_bound = -1;
        int upper_bound = len_ - 1;
        if (kth_check < kth) {
            return -1;
        }
        while (upper_bound - lower_bound > 1) {
            int mid_bound = (lower_bound + upper_bound) / 2;
            int mid_sum = Sum(mid_bound);
            if (mid_sum < kth) {
                lower_bound = mid_bound;
            } else {
                upper_bound = mid_bound;
            }
        }
        return upper_bound;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int size, mmm, kkk;
    std::cin >> size;
    std::vector<int> numbers(size);
    std::cin >> mmm;
    std::cin >> kkk;
    for (int i = 0; i < size; ++i) {
        std::cin >> numbers[i];
    }
    std::string rlstring;
    std::cin >> rlstring;
    int index;
    std::vector<std::pair<int, int>> pos;
    for (size_t i = 0; i < numbers.size(); ++i) {
        pos.push_back(std::make_pair(numbers[i], i));
    }
    std::sort(pos.begin(), pos.end());
    int ind;
    Heap heap(size);
    std::vector<int> sort_pos(size);
    for (size_t i = 0; i < pos.size(); ++i) {
        sort_pos[pos[i].second] = i;
    }
    ind = sort_pos[0];
    for (; ind < heap.len_; ind = (ind | (ind + 1))) {
        heap.indicators_[ind] += 1;
    }
    std::vector<int> result;
    int right = 0, left = 0;
    int in;
    for (char curr_lett : rlstring) {
        if (curr_lett == 'L') {
            index = sort_pos[left];
            for (; index < heap.len_; index = (index | (index + 1))) {
                heap.indicators_[index] -= 1;
            }
            ++left;
        } else if (curr_lett == 'R') {
            ++right;
            in = sort_pos[right];
            for (; in < heap.len_; in = (in | (in + 1))) {
                heap.indicators_[in] += 1;
            }
        }
        int statistics = heap.GetStat(kkk);
        (statistics == -1) ? result.push_back(-1)
                           : result.push_back(numbers[pos[statistics].second]);
    }
    for (int x : result) {
        std::cout << x << "\n";
    }
}
*/
