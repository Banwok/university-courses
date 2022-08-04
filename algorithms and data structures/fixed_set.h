#include <vector>
#include <list>

class FixedSet {
public:
    FixedSet() = default;

    std::vector<std::list<int>> mem_;
    int Hash(int pos) const {
        int res = pos % (mem_.size());
        int hash = std::abs(res);
        return hash;
    }

    void Initialize(const std::vector<int>& numbers) {
        if (numbers.empty()) {
            return;
        }
        mem_.resize(0);
        mem_.resize(numbers.size());
        for (auto& i : numbers) {
            mem_[Hash(i)].push_back(i);
        }
    }

    bool Contains(int number) const {
        if (!mem_.empty()) {
            for (auto& i : mem_[Hash(number)]) {
                if (i == number) {
                    return true;
                }
            }
        } else {
            return false;
        }
        return false;
    }
};
