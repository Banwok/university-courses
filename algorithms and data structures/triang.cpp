/*
#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <algorithm>
#include <list>

class Hash {
private:
    int a_, b_, c_;

public:
    Hash() {
        std::random_device generator;
        auto rand_int = std::uniform_int_distribution<int>(1, 1024);
        a_ = rand_int(generator);
        b_ = rand_int(generator);
        c_ = rand_int(generator);
    }
    size_t operator()(std::array<int, 3> array) {
        return a_ * static_cast<size_t>(array[0]) + b_ * static_cast<size_t>(array[1]) +
               c_ * static_cast<size_t>(array[2]);
    }
};

template <class Key>
class HashTable {
private:
    std::vector<std::list<Key>> table_;
    Hash hash_;
    size_t size_;
    const double max_factor = 2;
    const double rehash_factor = 2;

public:
    HashTable() : table_(1), size_(0){};

    void CheckResize() {
        if (size_ > table_.size() * max_factor) {
            this->resize(static_cast<size_t >(table_.size() * rehash_factor));
        }
    }

    size_t size() const {
        return size_;
    }

    void insert(const Key &key) {
        std::list<Key> &cell = table_[hash_(key) % table_.size()];
        if (std::find(cell.begin(), cell.end(), key) == cell.end()) {
            cell.push_back(key);
            ++size_;
        }
        CheckResize();
    }

    void resize(size_t new_size) {
        std::vector<std::list<Key>> new_table(new_size);
        std::swap(this->table_, new_table);
        size_ = 0;
        for (auto &cell : new_table) {
            for (auto &key : cell) {
                this->insert(key);
            }
        }
    }
};

inline int gcd(int side_b, int side_a) {
    return side_a ? gcd(side_a, side_b % side_a) : side_b;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t size;
    std::cin >> size;
    std::vector<std::array<int, 3>> triangles(size);
    for (auto &side : triangles) {
        std::cin >> side[0] >> side[1] >> side[2];
    }
    for (auto &triangle : triangles) {
        std::sort(triangle.begin(), triangle.end());
        int gcd_ab = gcd(triangle[0], triangle[1]);
        int gcd_abc = gcd(gcd_ab, triangle[2]);
        triangle[0] /= gcd_abc;
        triangle[1] /= gcd_abc;
        triangle[2] /= gcd_abc;
    }
    HashTable<std::array<int, 3>> hash_table;
    for (const auto &triangle : triangles) {
        hash_table.insert(triangle);
    }
    size_t ans = hash_table.size();
    std::cout << ans;
    return 0;
}
*/
