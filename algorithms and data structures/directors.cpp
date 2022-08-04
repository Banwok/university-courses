/*
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>

class Sets {
public:
    explicit Sets() = default;

    explicit Sets(size_t size) {
        if (size > 0) {
            parents_.resize(static_cast<unsigned int>(size));
            for (size_t i = 0; i < size; ++i) {
                parents_[i] = i;
            }
        }
    }

    ~Sets() = default;

    int FindSet(int vertex) {
        if (vertex != parents_[vertex]) {
            parents_[vertex] = FindSet(parents_[vertex]);
            return parents_[vertex];
        } else {
            return vertex;
        }
    }

    int UnionSets(int first, int second) {
        if (first < 0) {
            return 0;
        }
        int parent_f = FindSet(first);
        int parent_s = FindSet(second);
        if (!((first != second) and (second == parent_s) and (parent_f != parent_s))) {
            return 0;
        } else {
            parents_[parent_s] = parent_f;
            return 1;
        }
    }

protected:
    std::vector<int> parents_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::ifstream fin;
    fin.open("input.txt");
    size_t n_value;
    size_t m_value;
    fin >> n_value >> m_value;
    Sets my_set(n_value);
    std::string str;
    std::getline(fin, str);
    std::ofstream fout;
    fout.open("output.txt");
    for (size_t ind = 0; ind < m_value; ++ind) {
        std::stringstream ss;
        std::getline(fin, str);
        ss << str;
        int first = 0;
        int second = -1;
        ss >> first;
        --first;
        if (!ss.eof()) {
            ss >> second;
            --second;
        }
        if (second != -1) {
            fout << my_set.UnionSets(first, second);
        } else {
            fout << my_set.FindSet(first) + 1;
        }
        if (ind != m_value - 1) {
            fout << '\n';
        }
    }
    return 0;
}
*/
